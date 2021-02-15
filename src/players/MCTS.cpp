#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "Players.hpp"
#include <random>

move_t MCTSPlayer::selectMove(state_t state, colormode_t colorMode) {
  // TODO
  auto moves = gameStateHandler->allMoves(state);
  return *std::next(std::begin(moves), rand() % moves.size());
}
