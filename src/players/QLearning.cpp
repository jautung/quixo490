#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../utils/DataHandler.hpp"
#include "Players.hpp"
#include <Eigen/Core>
#include <random>
#include <tuple>
#include <vector>

extern std::mt19937 rng;

namespace {
  state_t initState = 0b0;
  long unsigned int maxDepth = 10000;   // prevents infinite loops when playing out each iteration
  double initWeight = 0.0;              // initial weights in the weight matrix ...
  double initWeightNoise = 0.0;         // ... perturbed by a uniform distribution of [-initWeightNoise, initWeightNoise]
  double learningRateInit = 0.1;        // initial learning rate
  double learningRateDecRatio = 0.999;  // factor for learning rate to decrease per iteration
  double discountRate = 0.999;          // temporal discount factor
  double epsilon = 0.001;               // epsilon in epsilon-greedy play out of each iteration
}

QLearningPlayer::QLearningPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler, int initInitIters, int initPerMoveIters) : Player(initGameStateHandler, initGraphicsHandler) {
  initIters = initInitIters;
  perMoveIters = initPerMoveIters;
  std::bernoulli_distribution initEpsilonDistri(epsilon);
  epsilonDistri = initEpsilonDistri;
}

QLearningPlayer::~QLearningPlayer() {}

move_t QLearningPlayer::selectMove(state_t state, colormode_t colorMode) {
  for (int i = 0; i < perMoveIters; i++) {
    runIter(state);
    learningRate *= learningRateDecRatio;
  }
  return gameStateHandler->allPotentialMovesCache[selectBestMoveIndex(state)];
}

void QLearningPlayer::clearCache() {
  auto testFeatures = getFeatures(initState);
  weights = Eigen::MatrixXd::Constant(gameStateHandler->allPotentialMovesCache.size(), testFeatures.size(), initWeight);
  weights += initWeightNoise * Eigen::MatrixXd::Random(gameStateHandler->allPotentialMovesCache.size(), testFeatures.size());
}

void QLearningPlayer::initLearn() {
  learningRate = learningRateInit;
  for (int i = 0; i < initIters; i++) {
    runIter(initState);
    learningRate *= learningRateDecRatio;
  }
}

void QLearningPlayer::runIter(state_t state) {
  std::vector<std::tuple<state_t, int>> stateMoveStack;
  while (true) {
    if (gameStateHandler->containsLine(state, TILE_X) || gameStateHandler->containsLine(state, TILE_O) || stateMoveStack.size() == maxDepth) {
      updateWeights(stateMoveStack, state);
      break;
    }
    auto moveIndex = selectQMoveIndex(state);
    stateMoveStack.push_back(std::make_tuple(state, moveIndex));
    auto move = gameStateHandler->allPotentialMovesCache[moveIndex];
    state = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
  }
}

void QLearningPlayer::updateWeights(std::vector<std::tuple<state_t, int>> &stateMoveStack, state_t finalState) {
  state_t nextState = finalState;
  for (auto rIter = stateMoveStack.rbegin(); rIter != stateMoveStack.rend(); rIter++) {
    auto currState = std::get<0>(*rIter);
    auto moveIndex = std::get<1>(*rIter);
    double optFutureVal;
    if (gameStateHandler->containsLine(nextState, TILE_X)) { // next state is terminal winning (i.e. losing for current player)
      optFutureVal = -1;
    } else if (gameStateHandler->containsLine(nextState, TILE_O)) { // next state is terminal losing (i.e. winning for current player)
      optFutureVal = 1;
    } else {
      Eigen::VectorXd nextQs = weights * getFeatures(nextState);
      optFutureVal = -nextQs.minCoeff();
    }
    Eigen::VectorXd currFeatures = getFeatures(currState);
    auto currQs = weights * currFeatures;
    auto currVal = currQs(moveIndex);
    Eigen::VectorXd weightsForMove = weights(moveIndex, Eigen::all); // one row of the weights matrix
    weightsForMove = weightsForMove + learningRate * discountRate * (optFutureVal - currVal) * currFeatures;
    weights.row(moveIndex) = weightsForMove.transpose(); // replacing row of the weights matrix
    nextState = currState;
  }
}

int QLearningPlayer::selectQMoveIndex(state_t state) {
  if (epsilonDistri(rng)) { // epsilon ...
    auto movesIndices = gameStateHandler->allMovesIndices(state);
    std::uniform_int_distribution<int> dist(0, movesIndices.size() - 1);
    return movesIndices[dist(rng)];
  } else { // ... greedy
    return selectBestMoveIndex(state);
  }
}

int QLearningPlayer::selectBestMoveIndex(state_t state) {
  Eigen::VectorXd qs = weights * getFeatures(state);
  auto movesIndices = gameStateHandler->allMovesIndices(state); // legal moves
  int j = 0;
  for (int i = 0; i < qs.size(); i++) { // overwrite qs(i) by -INFINITY for each i not in movesIndices (i.e. illegal moves)
    if (movesIndices[j] == i) {
      j++;
    } else {
      qs(i) = -INFINITY;
    }
  }
  int moveIndexBest; // find index of best move among legal moves
  qs.maxCoeff(&moveIndexBest);
  return moveIndexBest;
}

Eigen::VectorXd QLearningPlayer::getFeatures(state_t state) {
  auto len = gameStateHandler->len;
  Eigen::VectorXd features(1 + 2 + 4*len + 4);
  int featureIndex = 0;

  // constant bias term
  features[featureIndex++] = 1;

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
