#include "../game/GameStateHandler.hpp"
#include "NcrCalculator.hpp"
#include "OptComputer.hpp"
#include "OrdCalculator.hpp"
#include <iostream>

namespace {
  nbit_t stateNBits = 64;
  nbit_t halfStateNBits = stateNBits/2;
  state_t halfStateMask = 0b11111111111111111111111111111111;
}

OptComputer::OptComputer(nbit_t initNumTiles, GameStateHandler* initGameStateHandler) {
  numTiles = initNumTiles;
  gameStateHandler = initGameStateHandler;
  ncrCalculator = new NcrCalculator(numTiles);
  ordCalculator = new OrdCalculator(numTiles);
}

OptComputer::~OptComputer() {
  delete ncrCalculator;
  delete ordCalculator;
}

void OptComputer::computeAll() {
  for (nbit_t numUsed = numTiles; numUsed >= 0; numUsed--) {
    for (nbit_t numX = 0; numX <= numUsed; numX++) {
      nbit_t numO = numUsed - numX;
      computeClass(numX, numO); // TODO: add a way to load information from previous classes
      return; // TODO: remove
    }
  }
}

void OptComputer::computeClass(nbit_t numX, nbit_t numO) { // value iteration
  numX = 13; // TODO: remove
  numO = 12;

  sindex_t numStates = ncrCalculator->ncr(numTiles, numX) * ncrCalculator->ncr(numTiles-numX, numO);
  sindex_t numStatesMax = numX != numO ? 2*numStates : numStates;
  std::vector<result_t> results;
  results.reserve(numStatesMax); // (numX=numX, numO=numO) for 0~numStates-1; maybe (numX=numO, numO=numX) for numStates~2*numStates-1

  std::cerr << "pre-computation done; beginning value iteration (numStates=" << numStates << ", numStatesMax=" << numStatesMax << ")\n";
  for (sindex_t stateIndex = 0; stateIndex < numStatesMax; stateIndex++) {
    auto state = indexToState(stateIndex, numX, numO, numStates);
    if (gameStateHandler->containsLine(state, TILE_X)) {
      results[stateIndex] = RESULT_WIN;
    } else if (gameStateHandler->containsLine(state, TILE_O)) {
      results[stateIndex] = RESULT_LOSS;
    } else {
      results[stateIndex] = RESULT_DRAW;
    }
  }

  std::cerr << "initial scan done\n";
  while (true) {
    std::cerr << "one iteration starts...\n";
    bool updateMade = false;
    for (sindex_t stateIndex = 0; stateIndex < numStatesMax; stateIndex++) {
      if (results[stateIndex] != RESULT_DRAW) {
        continue;
      }
      auto state = indexToState(stateIndex, numX, numO, numStates);
      auto moves = gameStateHandler->allMoves(state);
      bool allChildrenWin = true;
      for (auto move : moves) {
        auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
        auto childStateIndex = stateToIndex(childState);
        if (numX != numO && stateIndex < numStates) {
          childStateIndex += numStates;
        }
        if (results[childStateIndex] == RESULT_LOSS) {
          results[stateIndex] = RESULT_WIN;
          updateMade = true;
          allChildrenWin = false;
          break;
        } else if (results[childStateIndex] == RESULT_DRAW) {
          allChildrenWin = false;
        }
      }
      if (allChildrenWin) {
        results[stateIndex] = RESULT_LOSS;
        updateMade = true;
      }
    }
    std::cerr << "one iteration ends...\n";
    if (!updateMade) {
      break;
    }
  }

  // TODO: remove
  std::cout << "INDIRECT WINS:\n";
  for (sindex_t stateIndex = 0; stateIndex < numStatesMax; stateIndex++) {
    if (results[stateIndex] == RESULT_WIN) {
      auto state = indexToState(stateIndex, numX, numO, numStates);
      if (!gameStateHandler->containsLine(state, TILE_X)) {
        gameStateHandler->print(state);
        std::cout << "\n";
      }
    }
  }
  std::cout << "INDIRECT LOSSES:\n";
  for (sindex_t stateIndex = 0; stateIndex < numStatesMax; stateIndex++) {
    if (results[stateIndex] == RESULT_LOSS) {
      auto state = indexToState(stateIndex, numX, numO, numStates);
      if (!gameStateHandler->containsLine(state, TILE_O)) {
        gameStateHandler->print(state);
        std::cout << "\n";
      }
    }
  }
  std::cout << "DRAWS:\n";
  for (sindex_t stateIndex = 0; stateIndex < numStatesMax; stateIndex++) {
    if (results[stateIndex] == RESULT_DRAW) {
      auto state = indexToState(stateIndex, numX, numO, numStates);
      gameStateHandler->print(state);
      std::cout << "\n";
    }
  }

  // TODO: save results
}

state_t OptComputer::indexToState(sindex_t stateIndex, nbit_t numX, nbit_t numO, sindex_t numStates) {
  if (stateIndex >= numStates) {
    stateIndex -= numStates;
    std::swap(numX, numO);
  }
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
  nbit_t numX = __builtin_popcount(xState);
  nbit_t numO = __builtin_popcount(oFilteredOrd);
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
