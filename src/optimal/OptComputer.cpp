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

      std::vector<result4_t> resultsCacheNormPlus;
      std::vector<result4_t> resultsCacheFlipPlus;
      sindex_t numCacheStatesLoadedTotal = 0;
      if (memoryChecker) memoryChecker->checkVmRss("Before loading for class (" + std::to_string(numA) + ", " + std::to_string(numB) + ")");
      if (numUsed != numTiles) {
        resultsCacheNormPlus = dataHandler->loadClass(gameStateHandler->len, numB, numA+1); // (numA, numB) -- +1 --> (numB, numA+1)
        if (memoryChecker) {
          auto numCacheStatesLoaded = numStatesClass(numA+1, numB);
          memoryChecker->checkVector(&(*resultsCacheNormPlus.begin()), &(*resultsCacheNormPlus.end()), "resultsCacheNormPlus (" + std::to_string(numCacheStatesLoaded) + " states)");
          numCacheStatesLoadedTotal += numCacheStatesLoaded;
        }
        if (numA != numB) {
          resultsCacheFlipPlus = dataHandler->loadClass(gameStateHandler->len, numA, numB+1); // (numB, numA) -- +1 --> (numA, numB+1)
          if (memoryChecker) {
            auto numCacheStatesLoaded = numStatesClass(numA, numB+1);
            memoryChecker->checkVector(&(*resultsCacheFlipPlus.begin()), &(*resultsCacheFlipPlus.end()), "resultsCacheFlipPlus (" + std::to_string(numCacheStatesLoaded) + " states)");
            numCacheStatesLoadedTotal += numCacheStatesLoaded;
          }
        }
      }
      if (memoryChecker) memoryChecker->checkVmRss("Loaded " + std::to_string(numCacheStatesLoadedTotal) + " cached states for class (" + std::to_string(numA) + ", " + std::to_string(numB) + ")");

      computeClass(numA, numB, resultsCacheNormPlus, resultsCacheFlipPlus);
    }
    if (numUsed == 0) {
      break;
    }
  }
}

sindex_t OptComputer::numStatesClass(nbit_t numA, nbit_t numB) {
  return ncrCalculator->ncr(numTiles, numA) * ncrCalculator->ncr(numTiles-numA, numB);
}

void OptComputer::computeClass(nbit_t numA, nbit_t numB, std::vector<result4_t> resultsCacheNormPlus, std::vector<result4_t> resultsCacheFlipPlus) { // value iteration
  auto startTime = std::chrono::high_resolution_clock::now();

  sindex_t numStates = numStatesClass(numA, numB);
  std::vector<result4_t> resultsNorm;
  std::vector<result4_t> resultsFlip;
  resultsNorm.reserve(numStates/4);
  if (numA != numB) {
    resultsFlip.reserve(numStates/4);
  }

  #pragma omp parallel
  {
    #pragma omp single
    {
      initClass(numA, numB, resultsNorm);
      if (numA != numB) {
        initClass(numB, numA, resultsFlip);
      }
      #pragma omp taskwait

      if (numA != numB) {
        checkTerminalsClass(numA, numB, resultsNorm, resultsFlip);
        checkTerminalsClass(numB, numA, resultsFlip, resultsNorm);
      } else {
        checkTerminalsClass(numA, numB, resultsNorm, resultsNorm);
      }
      #pragma omp taskwait

      parentLinkCacheClass(numA, numB, resultsNorm, resultsCacheNormPlus); // parent link optimization
      if (numA != numB) {
        parentLinkCacheClass(numB, numA, resultsFlip, resultsCacheFlipPlus);
      }
      #pragma omp taskwait

      while (true) {
        bool updateMade = false;
        if (numA != numB) {
          valueIterateClass(numA, numB, resultsNorm, resultsFlip, resultsCacheNormPlus, updateMade); // resultsReference of resultsNorm is resultsFlip for MKIND_ZERO and resultsCacheNormPlus for MKIND_PLUS
          valueIterateClass(numB, numA, resultsFlip, resultsNorm, resultsCacheFlipPlus, updateMade); // resultsReference of resultsFlip is resultsNorm for MKIND_ZERO and resultsCacheFlipPlus for MKIND_PLUS
        } else {
          valueIterateClass(numA, numB, resultsNorm, resultsNorm, resultsCacheNormPlus, updateMade); // resultsReference of resultsNorm is resultsNorm for MKIND_ZERO and resultsCacheNormPlus for MKIND_PLUS
        }
        #pragma omp taskwait
        if (!updateMade) {
          break;
        }
      }

      elimWinOrDrawClass(numA, numB, resultsNorm);
      if (numA != numB) {
        elimWinOrDrawClass(numB, numA, resultsFlip);
      }
      #pragma omp taskwait
    }
  }

  auto endTime = std::chrono::high_resolution_clock::now();
  std::cout << "Class (" << +numA << ", " << +numB << ") compute time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count()/1000.0 << "\n";

  dataHandler->saveClass(resultsNorm, gameStateHandler->len, numA, numB);
  if (numA != numB) {
    dataHandler->saveClass(resultsFlip, gameStateHandler->len, numB, numA);
  }
  std::cout << "Class (" << +numA << ", " << +numB << ") finished saving!\n";
}

void OptComputer::initClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results) {
  sindex_t numStates = numStatesClass(numX, numO);
  #pragma omp task shared(results)
  {
    for (sindex_t stateIndex = 0; stateIndex < numStates/4; stateIndex++) {
      results.push_back(RESULT_DRAW | RESULT_DRAW << 2 | RESULT_DRAW << 4 | RESULT_DRAW << 6);
    }
    if (numStates%4 != 0) {
      results.push_back(0b0);
      for (sindex_t stateIndex = 0; stateIndex < numStates%4; stateIndex++) {
        results.back() |= (RESULT_DRAW << (2 * (stateIndex)));
      }
    }
  }
}

void OptComputer::checkTerminalsClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther) {
  sindex_t numStates = numStatesClass(numX, numO);
  for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
    #pragma omp task shared(results, resultsOther)
    {
      auto state = indexToState(stateIndex, numX, numO);
      if (gameStateHandler->containsLine(state, TILE_X)) {
        dataHandler->setResult(results, stateIndex, RESULT_WIN);
      } else if (gameStateHandler->containsLine(state, TILE_O)) {
        dataHandler->setResult(results, stateIndex, RESULT_LOSS);
        for (auto parentState : gameStateHandler->allZeroParents(state)) { // parent link optimization
          auto parentStateIndex = stateToIndex(parentState);
          if (dataHandler->getResult(resultsOther, parentStateIndex) == RESULT_DRAW) {
            dataHandler->setResult(resultsOther, parentStateIndex, RESULT_WIN);
          }
        }
      }
    }
  }
}

void OptComputer::parentLinkCacheClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsCachePlus) {
  sindex_t numChildStates = numStatesClass(numO, numX+1);
  for (sindex_t childStateIndex = 0; childStateIndex < numChildStates; childStateIndex++) {
    auto childResult = dataHandler->getResult(resultsCachePlus, childStateIndex);
    if (childResult == RESULT_LOSS) {
      #pragma omp task shared(results, resultsCachePlus)
      {
        auto childState = indexToState(childStateIndex, numO, numX+1);
        for (auto state : gameStateHandler->allPlusParents(childState)) {
          auto stateIndex = stateToIndex(state);
          if (dataHandler->getResult(results, stateIndex) == RESULT_DRAW) {
            dataHandler->setResult(results, stateIndex, RESULT_WIN);
          }
        }
      }
    } else if (childResult == RESULT_DRAW) {
      #pragma omp task shared(results, resultsCachePlus)
      {
        auto childState = indexToState(childStateIndex, numO, numX+1);
        for (auto state : gameStateHandler->allPlusParents(childState)) {
          auto stateIndex = stateToIndex(state);
          if (dataHandler->getResult(results, stateIndex) == RESULT_DRAW) {
            dataHandler->setResult(results, stateIndex, RESULT_WIN_OR_DRAW);
          }
        }
      }
    }
  }
}

void OptComputer::valueIterateClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther, std::vector<result4_t> &resultsCachePlus, bool &updateMade) {
  sindex_t numStates = numStatesClass(numX, numO);
  for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
    if (dataHandler->getResult(results, stateIndex) != RESULT_DRAW) {
      continue;
    }
    #pragma omp task shared(results, resultsOther, resultsCachePlus, updateMade)
    {
      auto state = indexToState(stateIndex, numX, numO);
      auto moves = gameStateHandler->allMoves(state);
      bool allChildrenWin = true;
      for (auto move : moves) {
        auto moveKind = gameStateHandler->moveHandler->getKind(move);
        auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
        auto childStateIndex = stateToIndex(childState);
        result_t childResult = RESULT_DRAW; // dummy
        if (moveKind == MKIND_ZERO) {
          childResult = dataHandler->getResult(resultsOther, childStateIndex);
        } else if (moveKind == MKIND_PLUS) {
          childResult = dataHandler->getResult(resultsCachePlus, childStateIndex);
        }
        if (childResult == RESULT_DRAW) { // childResult will never be RESULT_LOSS because of parent link optimization
          allChildrenWin = false;
          break;
        }
      }
      if (allChildrenWin) {
        dataHandler->setResult(results, stateIndex, RESULT_LOSS);
        updateMade = true;
        for (auto parentState : gameStateHandler->allZeroParents(state)) { // parent link optimization
          auto parentStateIndex = stateToIndex(parentState);
          if (dataHandler->getResult(resultsOther, parentStateIndex) == RESULT_DRAW) {
            dataHandler->setResult(resultsOther, parentStateIndex, RESULT_WIN);
          }
        }
      }
    }
  }
}

void OptComputer::elimWinOrDrawClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results) {
  sindex_t numStates = numStatesClass(numX, numO);
  for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
    if (dataHandler->getResult(results, stateIndex) == RESULT_WIN_OR_DRAW) {
      dataHandler->setResult(results, stateIndex, RESULT_DRAW);
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
