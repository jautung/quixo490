#include "GamePlay.hpp"
#include <chrono>
#include <iostream>
#include <thread>

winner_t play(GameState* gameState, Player* playerX, Player* playerO, int timeStepMs, bool graphics) {
  while (true) {
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
  }
}
