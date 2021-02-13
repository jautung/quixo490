#include "GamePlay.hpp"
#include <chrono>
#include <iostream>
#include <thread>

GamePlay::GamePlay(GameState* initGameState, Player* initPlayerX, Player* initPlayerO, int initTimeStepMs, bool initGraphics) {
  gameState = initGameState;
  playerX = initPlayerX;
  playerO = initPlayerO;
  timeStepMs = initTimeStepMs;
  graphics = initGraphics;
}

winner_t GamePlay::playTurn() {
  gameState->display(graphics);
  auto move = playerX->selectMove(gameState);
  std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMs));
  std::cout << "X's Move: " << move << "\n\n";
  gameState->makeMove(move);

  if (gameState->containsLine(TILE_O)) {
    gameState->display(graphics);
    return WINNER_O;
  } else if (gameState->containsLine(TILE_X)) {
    gameState->display(graphics);
    return WINNER_X;
  }

  gameState->display(graphics);
  gameState->swapPlayers();
  move = playerO->selectMove(gameState);
  std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMs));
  std::cout << "O's Move: " << move << "\n\n";
  gameState->makeMove(move);
  gameState->swapPlayers();

  if (gameState->containsLine(TILE_X)) {
    gameState->display(graphics);
    return WINNER_X;
  } else if (gameState->containsLine(TILE_O)) {
    gameState->display(graphics);
    return WINNER_O;
  }

  return WINNER_UNKNOWN;
}

winner_t GamePlay::playNTurns(int nTurns) {
  for (int i = 0; i < nTurns; i++) {
    auto winner = playTurn();
    if (winner == WINNER_X || winner == WINNER_O) {
      return winner;
    }
  }
  gameState->display(graphics);
  return WINNER_UNKNOWN;
}

winner_t GamePlay::playTillEnd() {
  while (true) {
    auto winner = playTurn();
    if (winner == WINNER_X || winner == WINNER_O) {
      return winner;
    }
  }
}
