#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "Players.hpp"
#include <random>

MCTSPlayer::MCTSPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler, int initIters) : Player(initGameStateHandler, initGraphicsHandler) {
  iters = initIters;
}

move_t MCTSPlayer::selectMove(state_t state, colormode_t colorMode) {
  auto moves = gameStateHandler->allMoves(state);
  return *std::next(std::begin(moves), rand() % moves.size());
}
