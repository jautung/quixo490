#include "GameState.hpp"
#include "Players.hpp"
#include <random>
#include <iostream>

Move PlayerRandom::selectMove(const GameState* gameState) {
  auto moves = gameState->allMoves();
  return *std::next(std::begin(moves), rand() % moves.size());
}
