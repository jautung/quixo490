#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../optimal/OptComputer.hpp"
#include "../utils/DataHandler.hpp"
#include "Players.hpp"
#include <random>
#include <vector>

OptimalPlayer::OptimalPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler) : Player(initGameStateHandler, initGraphicsHandler) {
  auto len = gameStateHandler->len;
  optComputer = new OptComputer(len*len, gameStateHandler);
  dataHandler = new DataHandler();
}

OptimalPlayer::~OptimalPlayer() {
  delete optComputer;
  delete dataHandler;
}

move_t OptimalPlayer::selectMove(state_t state, colormode_t colorMode) {
  auto moves = gameStateHandler->allMoves(state);
  auto result = evalState(state);

  if (result == RESULT_LOSS) { // make a random move since loss anyway
    return *std::next(std::begin(moves), rand() % moves.size());
  }

  result_t filterChildStateResult;
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
  return *std::next(std::begin(filteredMoves), rand() % filteredMoves.size());
}

result_t OptimalPlayer::evalState(state_t state) {
  auto len = gameStateHandler->len;
  auto numX = gameStateHandler->getNumX(state);
  auto numO = gameStateHandler->getNumO(state);
  auto stateIndex = optComputer->stateToIndex(state);
  return dataHandler->loadState(len, numX, numO, stateIndex);
}
