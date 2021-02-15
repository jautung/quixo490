#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "Players.hpp"
#include <random>

move_t RandomPlayer::selectMove(state_t state, colormode_t colorMode) {
  auto moves = gameStateHandler->allMoves(state);
  return *std::next(std::begin(moves), rand() % moves.size());
}
