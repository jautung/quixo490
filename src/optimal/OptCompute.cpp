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
      optComputeClass(numX, numO);
      return; // TODO: remove
    }
  }
}

void OptCompute::optComputeClass(int numX, int numO) {
  numX = 12; // TODO: remove
  numO = 13;
  int numStates = ncrCalculator->ncr(25, numX) * ncrCalculator->ncr(25-numX, numO);
  for (int stateIndex = 0; stateIndex < numStates; stateIndex++) {
    auto state = indexToState(stateIndex, numX, numO);
    auto gameState = new GameState(state);
    std::cerr << *gameState << "\n";
  }
  // generate all GameState's using GameState(state_t initState = 0b0) with this numX and numO
  // number of states = 25 C (numX + numO) * (numX + numO) C numX
  // create a unordered_map from these to WIN/LOSS/DRAW, initialize all to DRAW
  // iterate over each of these DRAW GameStates, do the VI stuff
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
