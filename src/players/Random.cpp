#include "../game/GameState.hpp"
#include "Players.hpp"
#include <random>

Move RandomPlayer::selectMove(const GameState* gameState) {
  auto moves = gameState->allMoves();
  return *std::next(std::begin(moves), rand() % moves.size());
}
