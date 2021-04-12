#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../optimal/OptComputer.hpp"
#include "../utils/DataHandler.hpp"
#include "Players.hpp"
#include <random>
#include <vector>

extern std::mt19937 rng;

OptimalPlayer::OptimalPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler) : Player(initGameStateHandler, initGraphicsHandler) {
  auto len = gameStateHandler->len;
  optComputer = new OptComputer(gameStateHandler);
  dataHandler = new DataHandler();
}

OptimalPlayer::~OptimalPlayer() {
  delete optComputer;
  delete dataHandler;
}

move_t OptimalPlayer::selectMove(state_t state, int turnNum, colormode_t colorMode) {
  auto moves = gameStateHandler->allMoves(state);
  auto result = evalState(state);

  if (result == RESULT_LOSS) { // make a random move since loss anyway
    std::uniform_int_distribution<int> dist(0, moves.size() - 1);
    return moves[dist(rng)];
  }

  result_t filterChildStateResult = RESULT_LOSS; // dummy
  if (result == RESULT_WIN) {
    filterChildStateResult = RESULT_LOSS; // all children state with loss are good
  } else if (result == RESULT_DRAW) {
    filterChildStateResult = RESULT_DRAW; // can only hope for children state with draw
  }

  std::vector<move_t> filteredMoves;
  filteredMoves.reserve(moves.size());
  for (auto move : moves) {
    auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
    if (evalState(childState) == filterChildStateResult) {
      filteredMoves.push_back(move);
    }
  }

  // make a random move that has desired child state
  std::uniform_int_distribution<int> dist(0, filteredMoves.size() - 1);
  return filteredMoves[dist(rng)];
}

result_t OptimalPlayer::evalState(state_t state) {
  auto len = gameStateHandler->len;
  auto numX = gameStateHandler->getNumX(state);
  auto numO = gameStateHandler->getNumO(state);
  auto stateIndex = optComputer->stateToIndex(state);
  return dataHandler->loadState(len, numX, numO, stateIndex);
}
