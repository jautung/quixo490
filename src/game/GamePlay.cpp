#include "GamePlay.hpp"
#include "Graphics.hpp"
#include <chrono>
#include <iostream>
#include <thread>

GamePlay::GamePlay(GameState* initGameState, Player* initPlayerX, Player* initPlayerO, int initTimeStepMs, Graphics* initGraphics) {
  gameState = initGameState;
  playerX = initPlayerX;
  playerO = initPlayerO;
  timeStepMs = initTimeStepMs;
  graphics = initGraphics;
}

winner_t GamePlay::playTurn() {
  displayGameState();
  auto move = playerX->selectMove(gameState);
  if (!dynamic_cast<InteractivePlayer*>(playerX)) { // don't pause for interactive player
    std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMs));
  }
  std::cout << "X's Move: " << move << "\n\n";
  gameState->makeMove(move);

  if (gameState->containsLine(TILE_O)) {
    displayGameState();
    return WINNER_O;
  } else if (gameState->containsLine(TILE_X)) {
    displayGameState();
    return WINNER_X;
  }

  displayGameState();
  gameState->swapPlayers();
  move = playerO->selectMove(gameState, true);
  if (!dynamic_cast<InteractivePlayer*>(playerO)) { // don't pause for interactive player
    std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMs));
  }
  std::cout << "O's Move: " << move << "\n\n";
  gameState->makeMove(move);
  gameState->swapPlayers();

  if (gameState->containsLine(TILE_X)) {
    displayGameState();
    return WINNER_X;
  } else if (gameState->containsLine(TILE_O)) {
    displayGameState();
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
  displayGameState();
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

void GamePlay::displayGameState() {
  if (!graphics) {
    std::cout << *gameState;
  } else {
    graphics->drawBoard(gameState);
  }
}
