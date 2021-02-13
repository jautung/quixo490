#include "game/GamePlay.hpp"
#include "game/GameState.hpp"
#include "game/Graphics.hpp"
#include "optimal/OptComputeMain.hpp"
#include "players/Players.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <tclap/CmdLine.h>

Player* getPlayer(std::string playerType);

int main(int argc, char* argv[]) {
  testGraphics();
  exit(1);
  try {
    TCLAP::CmdLine cmd("Quixo Project");
    TCLAP::ValueArg<std::string> progArg("p", "program", "Program to run (`play`, `opt-compute`, or `mcts-compute`)", false, "play", "string", cmd);
    TCLAP::ValueArg<std::string> player1TypeArg("1", "player1", "For `play` program: player 1 type (`random`, `opt`, or `mcts`)", false, "random", "string", cmd);
    TCLAP::ValueArg<std::string> player2TypeArg("2", "player2", "For `play` program: player 2 type (`random`, `opt`, or `mcts`)", false, "random", "string", cmd);
    cmd.parse(argc, argv);
    auto prog = progArg.getValue();
    auto player1Type = player1TypeArg.getValue();
    auto player2Type = player2TypeArg.getValue();

    if (prog == "play") {
      auto now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());
      srand(now.time_since_epoch().count());
      auto player1 = getPlayer(player1Type);
      auto player2 = getPlayer(player1Type);
      auto gameState = new GameState();
      play(gameState, player1, player2);
    } else if (prog == "opt-compute") {
      optComputeMain();
    } else if (prog == "mcts-compute") {
      // TODO
    }

  } catch (TCLAP::ArgException const &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << "\n";
  }

  return 0;
}

Player* getPlayer(std::string playerType) {
  if (playerType == "random") {
    return new RandomPlayer();
  } else if (playerType == "opt") {
    return new OptimalPlayer();
  } else if (playerType == "mcts") {
    return new MCTSPlayer();
  } else {
    std::cerr << "error: " << "unknown player type: " << playerType << "\n";
    exit(1);
  }
}
