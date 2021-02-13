#pragma once
#include "../players/Players.hpp"
#include "GameState.hpp"
#include "Graphics.hpp"

enum winner_t {
  WINNER_X,
  WINNER_O,
  WINNER_UNKNOWN
};

class GamePlay {
  public:
    GamePlay(GameState* initGameState, Player* initPlayerX, Player* initPlayerO, int initTimeStepMs, Graphics* initGraphics);
    winner_t playTurn();
    winner_t playNTurns(int nTurns);
    winner_t playTillEnd();
  private:
    void displayGameState();
    GameState* gameState;
    Player* playerX;
    Player* playerO;
    int timeStepMs;
    Graphics* graphics;
};
