#pragma once
#include "../players/Players.hpp"
#include "GameStateHandler.hpp"
#include "GraphicsHandler.hpp"
#include <cstdint>

enum winner_t : uint8_t {
  WINNER_X,
  WINNER_O,
  WINNER_UNKNOWN
};

class GamePlayHandler {
  public:
    GamePlayHandler(Player* initPlayerX, Player* initPlayerO, int initTimePauseMs, GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler);
    void startGame(state_t initState = 0b0);
    winner_t playTurn();
    winner_t playNTurns(int nTurns);
    winner_t playTillEnd();
  private:
    Player* playerX;
    Player* playerO;
    int timePauseMs;
    GameStateHandler* gameStateHandler;
    GraphicsHandler* graphicsHandler;
    state_t state;
    void displayGameState();
};
