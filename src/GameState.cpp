#include "GameState.hpp"
#include <iostream>

GameState::GameState(int init) {
  board = init;
}

GameState::~GameState() {
}

int GameState::play() {
  std::cout << "Play!\n";
  return 1;
}
