#pragma once
#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../optimal/OptComputer.hpp"
#include "../utils/DataHandler.hpp"
#include <Eigen/Core>
#include <random>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Player {
  public:
    explicit Player(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler = NULL) {
      gameStateHandler = initGameStateHandler;
      graphicsHandler = initGraphicsHandler;
    }
    virtual ~Player() {}
    virtual move_t selectMove(state_t state, colormode_t colorMode = COLOR_NORM) {
      return gameStateHandler->moveHandler->create(DIR_UNDEFINED, 0, 0);
    }
    virtual void clearCache() {}
    virtual void initLearn() {}
  protected:
    GameStateHandler* gameStateHandler;
    GraphicsHandler* graphicsHandler;
};

class RandomPlayer : public Player {
  using Player::Player;
  public:
    move_t selectMove(state_t state, colormode_t colorMode) override;
};

class InteractivePlayer : public Player {
  using Player::Player;
  public:
    move_t selectMove(state_t state, colormode_t colorMode) override;
};

class OptimalPlayer : public Player {
  public:
    OptimalPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler = NULL);
    ~OptimalPlayer();
    move_t selectMove(state_t state, colormode_t colorMode) override;
    result_t evalState(state_t state);
  private:
    OptComputer* optComputer;
    DataHandler* dataHandler;
};

class HeuristicSimplePlayer : public Player {
  using Player::Player;
  public:
    move_t selectMove(state_t state, colormode_t colorMode) override;
};

class MCTSPlayer : public Player {
  public:
    MCTSPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler = NULL, int initInitIters = 0, int initPerMoveIters = 0);
    ~MCTSPlayer();
    move_t selectMove(state_t state, colormode_t colorMode) override;
    void clearCache() override;
    void initLearn() override;
  private:
    int initIters;
    int perMoveIters;
    std::unordered_map<state_t, std::tuple<int, int>> cache; // map from state to (2 * number of visits, 2 * number of X wins + 1 * number of draws)
    void runIter(state_t state);
    void backPropagate(result_t result, std::vector<state_t> &traversedStates);
    std::tuple<std::unordered_set<state_t>, std::unordered_set<state_t>> getChildrenStates(state_t state); // returns (exploredChildrenStates, unexploredChildrenStates)
    void playout(state_t state, std::vector<state_t> &traversedStates);
    state_t selectBestUcbState(state_t state, std::unordered_set<state_t> &exploredChildrenStates);
    move_t selectBestMove(state_t state);
    std::tuple<int, int> addTuples(const std::tuple<int, int> &tupleA, const std::tuple<int, int> &tupleB);
    bool equalInfo(const std::tuple<int, int> &infoA, const std::tuple<int, int> &infoB);
    bool worseInfo(const std::tuple<int, int> &infoA, const std::tuple<int, int> &infoB);
};

class QLearningPlayer : public Player {
  public:
    QLearningPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler = NULL, int initInitIters = 0, int initPerMoveIters = 0);
    ~QLearningPlayer();
    move_t selectMove(state_t state, colormode_t colorMode) override;
    void clearCache() override;
    void initLearn() override;
  private:
    int initIters;
    int perMoveIters;
    Eigen::MatrixXd weights;
    double learningRate;
    std::bernoulli_distribution epsilonDistri;
    void runIter(state_t state);
    void updateWeights(std::vector<std::tuple<state_t, int>> &stateMoveStack, state_t finalState);
    int selectQMoveIndex(state_t state);
    int selectBestMoveIndex(state_t state);
    Eigen::VectorXd getFeatures(state_t state);
};
