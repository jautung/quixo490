#pragma once
#include "../game/GameStateHandler.hpp"

class OptAnalyzer {
  public:
    OptAnalyzer(GameStateHandler* initGameStateHandler);
    ~OptAnalyzer();
    void analyzeNumWinLossDrawStates(bool adjacentXOs = false);
  private:
    GameStateHandler* gameStateHandler;
    len_t len;
    OptComputer* optComputer;
    DataHandler* dataHandler;
};
