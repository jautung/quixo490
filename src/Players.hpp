#pragma once
#include "GameState.hpp"

class Player {
  public:
    virtual Move selectMove(const GameState gameState) {
      return Move(DIR_LEFT, 0, 0); // dummy
    }
};

class PlayerRandom : public Player {
  public:
    Move selectMove(const GameState gameState) override;
};

class PlayerMCTS : public Player {
  public:
    PlayerMCTS(int init);
    Move selectMove(const GameState gameState) override;
};

class PlayerOptimal : public Player {
  public:
    PlayerOptimal(int init);
    Move selectMove(const GameState gameState) override;
};
