#include "GameState.hpp"
#include "GamePlay.hpp"
#include "Players.hpp"
#include "OptCompute.hpp"
#include <iostream>

int main() {
  GameState gameState(NULL);
  PlayerRandom player1;
  PlayerRandom player2;
  play(gameState, &player1, &player2);
  // gameState.swapPlayers();
  // gameState.makeMove(Move(DIR_LEFT, 4, 0));
  // gameState.swapPlayers();
  // gameState.makeMove(Move(DIR_LEFT, 4, 0));
  // gameState.makeMove(Move(DIR_LEFT, 4, 0));
  // gameState.makeMove(Move(DIR_UP, 3, 0));
  // gameState.swapPlayers();
  // std::cout << gameState;
  // auto moves = gameState.allMoves();
  // for (const auto& move: moves) {
  //   std::cout << move;
  // }
  // gameState.makeMove(Move(DIR_UP, 2, 0));
  // gameState.makeMove(Move(DIR_UP, 2, 0));
  // gameState.makeMove(Move(DIR_UP, 2, 0));
  // gameState.makeMove(Move(DIR_UP, 2, 0));
  // std::cout << gameState;
  // std::cout << gameState.containsLine(TILE_X) << "\n";
  // std::cout << gameState.containsLine(TILE_O) << "\n";
  return 0;
}
