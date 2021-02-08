#include "../game/GameState.hpp"
#include "Players.hpp"
#include <chrono>
#include <random>

Move RandomPlayer::selectMove(const GameState* gameState) {
  auto now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());
  auto moves = gameState->allMoves();
  srand(now.time_since_epoch().count());
  return *std::next(std::begin(moves), rand() % moves.size());
}
