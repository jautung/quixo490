#include "GamePlay.hpp"
#include <iostream>
// #include <chrono>
// #include <thread>

void play(GameState* gameState, Player* player1, Player* player2) {
  while (true) {
    auto move = player1->selectMove(gameState);
    gameState->makeMove(move);
    gameState->swapPlayers();
    std::cout << *gameState << "\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    move = player2->selectMove(gameState);
    gameState->makeMove(move);
    gameState->swapPlayers();
    std::cout << *gameState << "\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    if (gameState->containsLine(TILE_X) || gameState->containsLine(TILE_O)) {
      break;
    }
  }
}
