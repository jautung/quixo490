#pragma once
#include "../players/Players.hpp"
#include "GameState.hpp"

enum winner_t {
  WINNER_X,
  WINNER_O
};

winner_t play(GameState* gameState, Player* playerX, Player* playerO, int timeStepMs, bool graphics);
