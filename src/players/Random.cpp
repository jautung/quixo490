#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "Players.hpp"
#include <random>

extern std::mt19937 rng;

move_t RandomPlayer::selectMove(state_t state, colormode_t colorMode) {
  auto moves = gameStateHandler->allMoves(state);
  std::uniform_int_distribution<int> dist(0, moves.size() - 1);
  return moves[dist(rng)];
}
