#pragma once
#include "../game/GameState.hpp"
#include "NcrCalculator.hpp"
#include "OrdCalculator.hpp"

enum result_t {
  RESULT_WIN,
  RESULT_LOSS,
  RESULT_DRAW
};

class OptCompute {
  public:
    void optComputeMain();
  private:
    NcrCalculator* ncrCalculator;
    OrdCalculator* ordCalculator;
    void optComputeClass(int numX, int numO);
    state_t indexToState(int stateIndex, int numX, int numO);
    int stateToIndex(state_t state);
    state_t unfilterOState(state_t oFilteredState, state_t xState);
    state_t filterOState(state_t oState, state_t xState);
};
