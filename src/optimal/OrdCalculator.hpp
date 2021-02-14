#pragma once
#include "../game/GameState.hpp"
#include <vector>

class OrdCalculator {
  public:
    OrdCalculator(int initBitLimit);
    state_t ordToState(int ord, int bitCount);
    int stateToOrd(state_t state);

  private:
    int bitLimit;
    int ordBitCountIndex(int ord, int bitCount);
    std::vector<state_t> ordToStateCache;
    std::vector<int> stateToOrdCache;
};
