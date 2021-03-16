#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "Players.hpp"
#include <random>

extern std::mt19937 rng;

move_t HeuristicSimplePlayer::selectMove(state_t state, colormode_t colorMode) {
  auto moves = gameStateHandler->allMoves(state);

  // quick check for one-move to terminal states
  std::vector<move_t> winningMoves;
  std::vector<move_t> nonLosingMoves;
  for (auto move : moves) {
    auto childState = gameStateHandler->makeMove(state, move);
    if (!gameStateHandler->containsLine(childState, TILE_O)) {
      if (gameStateHandler->containsLine(childState, TILE_X)) {
        winningMoves.push_back(move);
      }
      nonLosingMoves.push_back(move);
    }
  }
  if (winningMoves.size() > 0) {
    std::uniform_int_distribution<int> dist(0, winningMoves.size() - 1);
    return winningMoves[dist(rng)];
  }

  // implement simple heuristic among non-losing moves
  if (nonLosingMoves.size() > 0) {
    std::vector<move_t> plusMoves;
    for (auto move : moves) {
      auto moveKind = gameStateHandler->moveHandler->getKind(move);
      if (moveKind == MKIND_PLUS) {
        plusMoves.push_back(move);
      }
    }
    if (plusMoves.size() > 0) { // at least one plus move
      std::uniform_int_distribution<int> dist(0, plusMoves.size() - 1);
      return plusMoves[dist(rng)];
    }
    std::uniform_int_distribution<int> dist(0, nonLosingMoves.size() - 1); // no plus moves
    return nonLosingMoves[dist(rng)];
  }

  // return random move otherwise (all losing moves)
  std::uniform_int_distribution<int> dist(0, moves.size() - 1);
  return moves[dist(rng)];
}
