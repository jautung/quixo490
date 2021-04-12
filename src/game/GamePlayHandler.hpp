#pragma once
#include "../players/Players.hpp"
#include "GameStateHandler.hpp"
#include "GraphicsHandler.hpp"
#include <chrono>
#include <cstdint>

enum winner_t : uint8_t {
  WINNER_X,
  WINNER_O,
  WINNER_UNKNOWN
};

class GamePlayHandler {
  public:
    GamePlayHandler(Player* initPlayerX, Player* initPlayerO, int initTimePauseMs, GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler = NULL, bool initSilent = false);
    void startGame(state_t initState = 0b0);
    winner_t playTurn();
    winner_t playNTurns(int nTurns, int& nTurnsPlayed);
    winner_t playTillEnd(int& nTurnsPlayed);
    std::chrono::high_resolution_clock::duration initTimeX;
    std::chrono::high_resolution_clock::duration initTimeO;
    std::chrono::high_resolution_clock::duration runTimeX;
    std::chrono::high_resolution_clock::duration runTimeO;
  private:
    Player* playerX;
    Player* playerO;
    int timePauseMs;
    GameStateHandler* gameStateHandler;
    GraphicsHandler* graphicsHandler;
    bool silent;
    state_t state;
    void displayGameState();
};
