#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../utils/DataHandler.hpp"
#include "Players.hpp"
#include <Eigen/Dense>
#include <iostream>

namespace {
}

QLearningPlayer::QLearningPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler) : Player(initGameStateHandler, initGraphicsHandler) {
  Eigen::MatrixXd m = Eigen::MatrixXd::Random(3,3);
  m = (m + Eigen::MatrixXd::Constant(3,3,1.2)) * 50;
  std::cout << "m =" << std::endl << m << std::endl;
  Eigen::VectorXd v(3);
  v << 1, 2, 3;
  std::cout << "m * v =" << std::endl << m * v << std::endl;
}

QLearningPlayer::~QLearningPlayer() {}

move_t QLearningPlayer::selectMove(state_t state, colormode_t colorMode) {
  return gameStateHandler->moveHandler->create(DIR_UNDEFINED, 0, 0);
}

void QLearningPlayer::clearCache() {
}
