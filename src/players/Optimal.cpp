#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../optimal/OptComputer.hpp"
#include "../utils/DataHandler.hpp"
#include "Players.hpp"
#include <random>
#include <tuple>
#include <vector>

extern std::mt19937 rng;

OptimalPlayer::OptimalPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler, double initErrorRate, bool initConsiderStepsQ) : Player(initGameStateHandler, initGraphicsHandler) {
  errorRate = initErrorRate;
  considerStepsQ = initConsiderStepsQ;
  auto len = gameStateHandler->len;
  optComputer = new OptComputer(gameStateHandler);
  dataHandler = new DataHandler();
}

OptimalPlayer::~OptimalPlayer() {
  delete optComputer;
  delete dataHandler;
}

move_t OptimalPlayer::selectMove(state_t state, colormode_t colorMode) {
  auto moves = gameStateHandler->allMoves(state);
  std::uniform_real_distribution<> errorDist(0, 1);
  if (errorDist(rng) < errorRate) { // optimal makes an error with probability errorRate
    std::uniform_int_distribution<int> dist(0, moves.size() - 1);
    return moves[dist(rng)];
  }

  auto resultTuple = evalState(state);
  auto result = std::get<0>(resultTuple);
  auto resultStep = std::get<1>(resultTuple);

  result_t filterChildStateResult = RESULT_DRAW; // dummy
  nsteps_t filterChildStateResultStep = resultStep - 1; // want a child state that has one less step (for win and loss states)

  if (result == RESULT_LOSS) {
    filterChildStateResult = RESULT_WIN; // all children state are win anyway
  } else if (result == RESULT_DRAW) {
    filterChildStateResult = RESULT_DRAW; // can only hope for children state with draw
  } else if (result == RESULT_WIN) {
    filterChildStateResult = RESULT_LOSS; // all children state with loss are good
  }

  std::vector<move_t> filteredMoves;
  filteredMoves.reserve(moves.size());
  for (auto move : moves) {
    auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
    auto childResultTuple = evalState(childState);
    auto childResult = std::get<0>(childResultTuple);
    auto childResultStep = std::get<1>(childResultTuple);
    if ((!considerStepsQ && childResult == filterChildStateResult) || (considerStepsQ && childResult == filterChildStateResult && (result == RESULT_DRAW || childResultStep == filterChildStateResultStep))) {
      filteredMoves.push_back(move);
    }
  }

  // make a random move that has desired child state
  std::uniform_int_distribution<int> dist(0, filteredMoves.size() - 1);
  return filteredMoves[dist(rng)];
}

std::tuple<result_t, nsteps_t> OptimalPlayer::evalState(state_t state) {
  auto len = gameStateHandler->len;
  auto numX = gameStateHandler->getNumX(state);
  auto numO = gameStateHandler->getNumO(state);
  auto stateIndex = optComputer->stateToIndex(state);
  return dataHandler->loadState(len, numX, numO, stateIndex, considerStepsQ);
}
