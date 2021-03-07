#pragma once
#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../optimal/OptComputer.hpp"
#include "../utils/DataHandler.hpp"
#include <tuple>
#include <unordered_map>

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
    MCTSPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler = NULL, int initIters = 0);
    ~MCTSPlayer();
    move_t selectMove(state_t state, colormode_t colorMode) override;
  private:
    int iters;
    std::unordered_map<state_t, std::tuple<int, int>> cache; // map from state to (2 * number of visits, 2 * number of X wins + 1 * number of draws)
    void runIter(state_t state);
    move_t selectBestMove(state_t state);
    bool equalInfo(std::tuple<int, int> infoA, std::tuple<int, int> infoB);
    bool worseInfo(std::tuple<int, int> infoA, std::tuple<int, int> infoB);
};
