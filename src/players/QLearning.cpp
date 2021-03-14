#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../utils/DataHandler.hpp"
#include "Players.hpp"
#include <Eigen/Core>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

namespace {
  state_t initState = 0b0;
  long unsigned int maxDepth = 1000;
  double initWeight = 0.0;
  double epsilon = 0.001;
}

QLearningPlayer::QLearningPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler, int initInitIters, int initPerMoveIters) : Player(initGameStateHandler, initGraphicsHandler) {
  initIters = initInitIters;
  perMoveIters = initPerMoveIters;
}

QLearningPlayer::~QLearningPlayer() {}

move_t QLearningPlayer::selectMove(state_t state, colormode_t colorMode) {
  for (int i = 0; i < perMoveIters; i++) {
    runIter(state);
  }
  return selectBestMove(state);
}

void QLearningPlayer::clearCache() {
  auto len = gameStateHandler->len;
  auto testFeatures = getFeatures(initState);
  weights = Eigen::MatrixXd::Constant(12*len-16, testFeatures.size(), initWeight);
}

void QLearningPlayer::initLearn() {
  for (int i = 0; i < initIters; i++) {
    runIter(initState);
  }
}

void QLearningPlayer::runIter(state_t state) {
  std::vector<std::tuple<state_t, move_t>> stateMoveStack;
  while (true) {
    if (gameStateHandler->containsLine(state, TILE_X) || gameStateHandler->containsLine(state, TILE_O) || stateMoveStack.size() == maxDepth) {
      updateWeights(stateMoveStack, state);
      break;
    }
    auto move = selectQMove(state);
    stateMoveStack.push_back(std::make_tuple(state, move));
    state = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
  }
}

void QLearningPlayer::updateWeights(std::vector<std::tuple<state_t, move_t>> &stateMoveStack, state_t finalState) {
  // use alpha
  // state_t nextState = finalState;
  // for (auto rIter = stateMoveStack.rbegin(); rIter != stateMoveStack.rend(); rIter++) {
  //   auto stateMoveStack = *rIter;
  //   if (result == RESULT_WIN) {
  //     cache[stateMoveStack] = addTuples(cache[stateMoveStack], std::make_tuple(2, 2));
  //     result = RESULT_LOSS;
  //   } else if (result == RESULT_LOSS) {
  //     cache[stateMoveStack] = addTuples(cache[stateMoveStack], std::make_tuple(2, 0));
  //     result = RESULT_WIN;
  //   } else if (result == RESULT_DRAW) {
  //     cache[stateMoveStack] = addTuples(cache[stateMoveStack], std::make_tuple(2, 1));
  //   }
  // }
  //   if (gameStateHandler->containsLine(state, TILE_X)) { // terminal winning
  //     updateWeights(RESULT_WIN, stateMoveStack, state);
  //     break;
  //   } else if (gameStateHandler->containsLine(state, TILE_O)) { // terminal losing
  //     updateWeights(RESULT_LOSS, stateMoveStack, state);
  //     break;
  //   } else if (stateMoveStack.size() == maxDepth) {
  //     updateWeights(RESULT_DRAW, stateMoveStack, state);
  //     break;
  //   }
}

move_t QLearningPlayer::selectQMove(state_t state) {
  if ((double)rand()/(double)RAND_MAX < epsilon) { // epsilon ...
    auto moves = gameStateHandler->allMoves(state);
    return *std::next(std::begin(moves), rand() % moves.size());
  } else { // ... greedy
    return selectBestMove(state);
  }
}

move_t QLearningPlayer::selectBestMove(state_t state) {
  return gameStateHandler->moveHandler->create(DIR_UNDEFINED, 0, 0);
  // weights * getFeatures(state);
  // return index of max of these
  // need to filter by allMoves also
}

Eigen::VectorXd QLearningPlayer::getFeatures(state_t state) {
  auto len = gameStateHandler->len;
  Eigen::VectorXd features(2 + 4*len + 4);
  int featureIndex = 0;

  // number of Xs and Os
  features[featureIndex++] = (double)(gameStateHandler->getNumX(state)) / ((double)len * (double)len);
  features[featureIndex++] = (double)(gameStateHandler->getNumO(state)) / ((double)len * (double)len);

  // number in each row, column, and diagonal
  for (bindex_t rowIndex = 0; rowIndex < len; rowIndex++) {
    features[featureIndex++] = (double)(gameStateHandler->numInRow(state, TILE_X, rowIndex)) / (double)len;
    features[featureIndex++] = (double)(gameStateHandler->numInRow(state, TILE_O, rowIndex)) / (double)len;
  }
  for (bindex_t colIndex = 0; colIndex < len; colIndex++) {
    features[featureIndex++] = (double)(gameStateHandler->numInCol(state, TILE_X, colIndex)) / (double)len;
    features[featureIndex++] = (double)(gameStateHandler->numInCol(state, TILE_O, colIndex)) / (double)len;
  }
  features[featureIndex++] = (double)(gameStateHandler->numInDiag1(state, TILE_X)) / (double)len;
  features[featureIndex++] = (double)(gameStateHandler->numInDiag1(state, TILE_O)) / (double)len;
  features[featureIndex++] = (double)(gameStateHandler->numInDiag2(state, TILE_X)) / (double)len;
  features[featureIndex++] = (double)(gameStateHandler->numInDiag2(state, TILE_O)) / (double)len;

  return features;
}
