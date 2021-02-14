#include "../game/GameState.hpp"
#include "NcrCalculator.hpp"
#include "OrdCalculator.hpp"
#include "OptCompute.hpp"
#include <iostream>
#include <bitset>

void OptCompute::optComputeMain() {
  ncrCalculator = new NcrCalculator(25);
  ordCalculator = new OrdCalculator(25);
  for (int numUsed = 25; numUsed >= 0; numUsed--) {
    for (int numX = 0; numX <= numUsed; numX++) {
      int numO = numUsed - numX;
      optComputeClass(numX, numO); // TODO: add a way to load information from previous classes
      return; // TODO: remove
    }
  }
}

void OptCompute::optComputeClass(int numX, int numO) { // value iteration
  numX = 13; // TODO: remove
  numO = 12;

  int numStates = ncrCalculator->ncr(25, numX) * ncrCalculator->ncr(25-numX, numO);
  std::vector<result_t> results;
  results.reserve(numStates);

  for (int stateIndex = 0; stateIndex < numStates; stateIndex++) {
    auto state = indexToState(stateIndex, numX, numO);
    auto gameState = new GameState(state);
    if (gameState->containsLine(TILE_X)) {
      results[stateIndex] = RESULT_WIN;
    } else if (gameState->containsLine(TILE_O)) {
      results[stateIndex] = RESULT_LOSS;
    } else {
      results[stateIndex] = RESULT_DRAW;
    }
  }

  while (true) {
    bool updateMade = false;
    for (int stateIndex = 0; stateIndex < numStates; stateIndex++) {
      if (results[stateIndex] == RESULT_DRAW) {
        auto state = indexToState(stateIndex, numX, numO);
        auto gameState = new GameState(state);
        auto moves = gameState->allMoves();
        bool allChildrenWin = true;
        for (auto& move : moves) {
          auto gameStateChild = new GameState(state);
          gameStateChild->makeMove(move);
          auto stateIndexChild = stateToIndex(gameStateChild->state);
          if (results[stateIndexChild] == RESULT_LOSS) {
            allChildrenWin = false;
            results[stateIndex] = RESULT_WIN;
            updateMade = true;
            break;
          } else if (results[stateIndexChild] != RESULT_WIN) {
            allChildrenWin = false;
            break;
          }
        }
        if (allChildrenWin) {
          results[stateIndex] = RESULT_LOSS;
          updateMade = true;
        }
      }
    }
    if (!updateMade) {
      break;
    }
  }

  // TODO: remove
  std::cout << "Indirect losses\n";
  for (int stateIndex = 0; stateIndex < numStates; stateIndex++) {
    if (results[stateIndex] == RESULT_LOSS) {
      auto state = indexToState(stateIndex, numX, numO);
      auto gameState = new GameState(state);
      if (!gameState->containsLine(TILE_O)) {
        std::cout << *gameState << "\n";
      }
    }
  }

  // TODO: remove
  std::cout << "Draws\n";
  for (int stateIndex = 0; stateIndex < numStates; stateIndex++) {
    if (results[stateIndex] == RESULT_DRAW) {
      auto state = indexToState(stateIndex, numX, numO);
      auto gameState = new GameState(state);
      std::cout << *gameState << "\n";
    }
  }
}

state_t OptCompute::indexToState(int stateIndex, int numX, int numO) {
  int coeff = ncrCalculator->ncr(25-numX, numO);
  int xOrd = stateIndex / coeff;
  int oFilteredOrd = stateIndex % coeff;
  auto xState = ordCalculator->ordToState(xOrd, numX);
  auto oFilteredState = ordCalculator->ordToState(oFilteredOrd, numO);
  auto oState = unfilterOState(oFilteredState, xState);
  return xState << 32 | oState;
}

int OptCompute::stateToIndex(state_t state) {
  state_t xState = state >> 32;
  state_t oState = state & 0b11111111111111111111111111111111;
  state_t oFilteredState = filterOState(oState, xState);
  int xOrd = ordCalculator->stateToOrd(xState);
  int oFilteredOrd = ordCalculator->stateToOrd(oFilteredState);
  int numX = __builtin_popcount(xState);
  int numO = __builtin_popcount(oState);
  int coeff = ncrCalculator->ncr(25-numX, numO);
  return xOrd*coeff + oFilteredOrd;
}

state_t OptCompute::unfilterOState(state_t oFilteredState, state_t xState) {
  state_t mainPointer = 0b1;
  state_t filteredPointer = 0b1;
  state_t oState = 0b0;
  for (int i = 0; i < 25; i++, mainPointer <<= 1) {
    if ((xState & mainPointer) != mainPointer) { // 0 bit in xState, so shift filteredPointer
      if ((oFilteredState & filteredPointer) == filteredPointer) {
        oState |= mainPointer;
      }
      filteredPointer <<= 1;
    }
  }
  return oState;
}

state_t OptCompute::filterOState(state_t oState, state_t xState) {
  state_t mainPointer = 0b1;
  state_t filteredPointer = 0b1;
  state_t oFilteredState = 0b0;
  for (int i = 0; i < 25; i++, mainPointer <<= 1) {
    if ((xState & mainPointer) != mainPointer) {
      if ((oState & mainPointer) == mainPointer) {
        oFilteredState |= filteredPointer;
      }
      filteredPointer <<= 1;
    }
  }
  return oFilteredState;
}
