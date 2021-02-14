#pragma once
#include "../game/GameState.hpp"
#include "NcrCalculator.hpp"
#include "OrdCalculator.hpp"

class OptCompute {
  public:
    void optComputeMain();
  private:
    NcrCalculator* ncrCalculator;
    OrdCalculator* ordCalculator;
    void optComputeClass(int numX, int numO);
    state_t indexToState(int stateIndex, int numX, int numO);
    state_t unfilterOState(state_t oFilteredState, state_t xState);
};
