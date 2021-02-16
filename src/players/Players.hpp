#pragma once
#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../optimal/DataHandler.hpp"
#include "../optimal/OptComputer.hpp"

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
  private:
    OptComputer* optComputer;
    DataHandler* dataHandler;
    result_t evalState(state_t state);
};

class MCTSPlayer : public Player {
  using Player::Player;
  public:
    move_t selectMove(state_t state, colormode_t colorMode) override;
};
