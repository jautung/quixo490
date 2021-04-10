#pragma once
#include "../game/GameStateHandler.hpp"

class OptAnalyzer {
  public:
    OptAnalyzer(GameStateHandler* initGameStateHandler);
    ~OptAnalyzer();
    void analyzeNumWinLossDrawStates();
};
