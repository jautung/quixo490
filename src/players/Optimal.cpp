#include "../game/GameState.hpp"
#include "Players.hpp"
#include <random>

Move OptimalPlayer::selectMove(const GameState* gameState, bool flippedColorsQ) {
  // TODO
  auto moves = gameState->allMoves();
  return *std::next(std::begin(moves), rand() % moves.size());
}
