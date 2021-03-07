#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "Players.hpp"
#include <random>

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
    return *std::next(std::begin(winningMoves), rand() % winningMoves.size());
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
      return *std::next(std::begin(plusMoves), rand() % plusMoves.size());
    }
    return *std::next(std::begin(nonLosingMoves), rand() % nonLosingMoves.size()); // no plus moves
  }

  // return random move otherwise (all losing moves)
  return *std::next(std::begin(moves), rand() % moves.size());
}
