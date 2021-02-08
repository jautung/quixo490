#include "../game/GameState.hpp"
#include "Players.hpp"
#include <random>

Move MCTSPlayer::selectMove(const GameState* gameState) {
  // TODO
  auto moves = gameState->allMoves();
  return *std::next(std::begin(moves), rand() % moves.size());
}
