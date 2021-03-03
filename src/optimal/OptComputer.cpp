#include "../game/GameStateHandler.hpp"
#include "../utils/DataHandler.hpp"
#include "../utils/MemoryChecker.hpp"
#include "../utils/NcrCalculator.hpp"
#include "OptComputer.hpp"
#include "OrdCalculator.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

namespace {
  nbit_t stateNBits = 64;
  nbit_t halfStateNBits = stateNBits/2;
  state_t halfStateMask = 0b11111111111111111111111111111111;
}

OptComputer::OptComputer(nbit_t initNumTiles, GameStateHandler* initGameStateHandler, MemoryChecker* initMemoryChecker) {
  numTiles = initNumTiles;
  gameStateHandler = initGameStateHandler;
  memoryChecker = initMemoryChecker;
  ncrCalculator = new NcrCalculator(numTiles);
  ordCalculator = new OrdCalculator(numTiles);
  dataHandler = new DataHandler();
}

OptComputer::~OptComputer() {
  delete ncrCalculator;
  delete ordCalculator;
  delete dataHandler;
}

void OptComputer::computeAll() {
  if (memoryChecker) memoryChecker->checkVmRss("Initial");
  for (nbit_t numUsed = numTiles;; numUsed--) {
    for (nbit_t numA = 0; numA <= numUsed/2; numA++) {
      nbit_t numB = numUsed - numA;
      if (dataHandler->existsClass(gameStateHandler->len, numA, numB) && dataHandler->existsClass(gameStateHandler->len, numB, numA)) {
        std::cout << "Class (" << +numA << ", " << +numB << ") already computed\n";
        continue;
      }

      std::vector<result_t> resultsCacheNormPlus;
      std::vector<result_t> resultsCacheFlipPlus;
      sindex_t numCacheLoaded = 0;
      if (memoryChecker) memoryChecker->checkVmRss("Before loading for class (" + std::to_string(numA) + ", " + std::to_string(numB) + ")");
      if (numUsed != numTiles) {
        resultsCacheNormPlus = dataHandler->loadClass(gameStateHandler->len, numB, numA+1); // (numA, numB) -- +1 --> (numB, numA+1)
        if (memoryChecker) memoryChecker->checkVector(&(*resultsCacheNormPlus.begin()), &(*resultsCacheNormPlus.end()), "resultsCacheNormPlus");
        numCacheLoaded += resultsCacheNormPlus.size();
        if (numA != numB) {
          resultsCacheFlipPlus = dataHandler->loadClass(gameStateHandler->len, numA, numB+1); // (numB, numA) -- +1 --> (numA, numB+1)
          if (memoryChecker) memoryChecker->checkVector(&(*resultsCacheFlipPlus.begin()), &(*resultsCacheFlipPlus.end()), "resultsCacheFlipPlus");
          numCacheLoaded += resultsCacheFlipPlus.size();
        }
      }
      if (memoryChecker) memoryChecker->checkVmRss("Loaded " + std::to_string(numCacheLoaded) + " cached states for class (" + std::to_string(numA) + ", " + std::to_string(numB) + ")");

      computeClass(numA, numB, resultsCacheNormPlus, resultsCacheFlipPlus);
    }
    if (numUsed == 0) {
      break;
    }
  }
}

void OptComputer::computeClass(nbit_t numA, nbit_t numB, std::vector<result_t> resultsCacheNormPlus, std::vector<result_t> resultsCacheFlipPlus) { // value iteration
  auto startTime = std::chrono::high_resolution_clock::now();

  sindex_t numStates = ncrCalculator->ncr(numTiles, numA) * ncrCalculator->ncr(numTiles-numA, numB);
  std::vector<result_t> resultsNorm;
  std::vector<result_t> resultsFlip;
  resultsNorm.reserve(numStates);
  if (numA != numB) {
    resultsFlip.reserve(numStates);
  }

  initialScanClass(numA, numB, resultsNorm, numStates);
  if (numA != numB) {
    initialScanClass(numB, numA, resultsFlip, numStates);
  }

  while (true) {
    bool updateMade = false;
    if (numA != numB) {
      valueIterateClass(numA, numB, resultsNorm, numStates, resultsFlip, resultsCacheNormPlus, updateMade); // resultsReference of resultsNorm is resultsFlip for MKIND_ZERO and resultsCacheNormPlus for MKIND_PLUS
      valueIterateClass(numB, numA, resultsFlip, numStates, resultsNorm, resultsCacheFlipPlus, updateMade); // resultsReference of resultsFlip is resultsNorm for MKIND_ZERO and resultsCacheFlipPlus for MKIND_PLUS
    } else {
      valueIterateClass(numA, numB, resultsNorm, numStates, resultsNorm, resultsCacheNormPlus, updateMade); // resultsReference of resultsNorm is resultsNorm for MKIND_ZERO and resultsCacheNormPlus for MKIND_PLUS
    }
    if (!updateMade) {
      break;
    }
  }

  auto endTime = std::chrono::high_resolution_clock::now();
  std::cout << "Class (" << +numA << ", " << +numB << ") compute time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count()/1000.0 << "\n";

  dataHandler->saveClass(resultsNorm, gameStateHandler->len, numA, numB);
  if (numA != numB) {
    dataHandler->saveClass(resultsFlip, gameStateHandler->len, numB, numA);
  }
}

void OptComputer::initialScanClass(nbit_t numX, nbit_t numO, std::vector<result_t> &results, sindex_t numStates) {
  for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
    auto state = indexToState(stateIndex, numX, numO);
    if (gameStateHandler->containsLine(state, TILE_X)) {
      results.push_back(RESULT_WIN);
    } else if (gameStateHandler->containsLine(state, TILE_O)) {
      results.push_back(RESULT_LOSS);
    } else {
      results.push_back(RESULT_DRAW);
    }
  }
}

void OptComputer::valueIterateClass(nbit_t numX, nbit_t numO, std::vector<result_t> &results, sindex_t numStates, std::vector<result_t> &resultsOther, std::vector<result_t> &resultsCachePlus, bool &updateMade) {
  for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
    if (results[stateIndex] != RESULT_DRAW) {
      continue;
    }
    auto state = indexToState(stateIndex, numX, numO);
    auto moves = gameStateHandler->allMoves(state);
    bool allChildrenWin = true;
    for (auto move : moves) {
      auto moveKind = gameStateHandler->moveHandler->getKind(move);
      auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
      auto childStateIndex = stateToIndex(childState);
      std::vector<result_t> resultsReference;
      if (moveKind == MKIND_ZERO) {
        resultsReference = resultsOther;
      } else if (moveKind == MKIND_PLUS) {
        resultsReference = resultsCachePlus;
      }
      if (resultsReference[childStateIndex] == RESULT_LOSS) {
        results[stateIndex] = RESULT_WIN;
        updateMade = true;
        allChildrenWin = false;
        break;
      } else if (resultsReference[childStateIndex] == RESULT_DRAW) {
        allChildrenWin = false;
      }
    }
    if (allChildrenWin) {
      results[stateIndex] = RESULT_LOSS;
      updateMade = true;
    }
  }
}

state_t OptComputer::indexToState(sindex_t stateIndex, nbit_t numX, nbit_t numO) {
  sindex_t coeff = ncrCalculator->ncr(numTiles-numX, numO);
  ord_t xOrd = stateIndex / coeff;
  ord_t oFilteredOrd = stateIndex % coeff;
  auto xState = ordCalculator->ordPopToState(xOrd, numX);
  auto oFilteredState = ordCalculator->ordPopToState(oFilteredOrd, numO);
  auto oState = unfilterOState(oFilteredState, xState);
  return xState << halfStateNBits | oState;
}

sindex_t OptComputer::stateToIndex(state_t state) {
  auto xState = (state >> halfStateNBits) & halfStateMask;
  auto oState = state & halfStateMask;
  auto oFilteredState = filterOState(oState, xState);
  auto xOrd = ordCalculator->stateToOrd(xState);
  auto oFilteredOrd = ordCalculator->stateToOrd(oFilteredState);
  auto numX = gameStateHandler->getNumX(state);
  auto numO = gameStateHandler->getNumO(state);
  sindex_t coeff = ncrCalculator->ncr(numTiles-numX, numO);
  return xOrd*coeff + oFilteredOrd;
}

state_t OptComputer::unfilterOState(state_t oFilteredState, state_t xState) {
  state_t mainPointer = 0b1;
  state_t filteredPointer = 0b1;
  state_t oState = 0b0;
  for (nbit_t i = 0; i < numTiles; i++, mainPointer <<= 1) {
    if ((xState & mainPointer) != mainPointer) {
      if ((oFilteredState & filteredPointer) == filteredPointer) {
        oState |= mainPointer;
      }
      filteredPointer <<= 1;
    }
  }
  return oState;
}

state_t OptComputer::filterOState(state_t oState, state_t xState) {
  state_t mainPointer = 0b1;
  state_t filteredPointer = 0b1;
  state_t oFilteredState = 0b0;
  for (nbit_t i = 0; i < numTiles; i++, mainPointer <<= 1) {
    if ((xState & mainPointer) != mainPointer) {
      if ((oState & mainPointer) == mainPointer) {
        oFilteredState |= filteredPointer;
      }
      filteredPointer <<= 1;
    }
  }
  return oFilteredState;
}
