#pragma once
#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"

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
    ~RandomPlayer() {}
    move_t selectMove(state_t state, colormode_t colorMode) override;
};

class InteractivePlayer : public Player {
  using Player::Player;
  public:
    ~InteractivePlayer() {}
    move_t selectMove(state_t state, colormode_t colorMode) override;
};

class OptimalPlayer : public Player {
  using Player::Player;
  public:
    ~OptimalPlayer() {}
    move_t selectMove(state_t state, colormode_t colorMode) override;
};

class MCTSPlayer : public Player {
  using Player::Player;
  public:
    ~MCTSPlayer() {}
    move_t selectMove(state_t state, colormode_t colorMode) override;
};
