#include "GameState.hpp"
#include <iostream>

int main() {
  GameState gameState(NULL);
  gameState.swapPlayers();
  gameState.makeMove(Move(DIR_LEFT, 4, 0));
  gameState.swapPlayers();
  gameState.makeMove(Move(DIR_LEFT, 4, 0));
  gameState.makeMove(Move(DIR_LEFT, 4, 0));
  gameState.makeMove(Move(DIR_UP, 3, 0));
  gameState.swapPlayers();
  std::cout << gameState;
  auto moves = gameState.allMoves();
  for (const auto& move: moves) {
    std::cout << move;
  }
  return 0;
}
