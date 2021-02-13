#pragma once
#include "../game/GameState.hpp"
#include "../game/Graphics.hpp"

class Player {
  public:
    virtual Move selectMove(const GameState* gameState) {
      return Move(DIR_LEFT, 0, 0); // dummy
    }
};

class RandomPlayer : public Player {
  public:
    Move selectMove(const GameState* gameState) override;
};

class InteractivePlayer : public Player {
  public:
    InteractivePlayer(Graphics* initGraphics);
    Move selectMove(const GameState* gameState) override;
  private:
    Graphics* graphics;
};

class OptimalPlayer : public Player {
  public:
    Move selectMove(const GameState* gameState) override;
};

class MCTSPlayer : public Player {
  public:
    Move selectMove(const GameState* gameState) override;
};
