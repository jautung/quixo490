#include "game/GamePlay.hpp"
#include "game/GameState.hpp"
#include "optimal/OptComputeMain.hpp"
#include "players/Players.hpp"
#include <iostream>
#include <tclap/CmdLine.h>

void getPlayer(std::string playerType, Player* player);

int main(int argc, char* argv[]) {
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
      RandomPlayer player1;
      RandomPlayer player2;
      // TODO: bugged, maybe we need malloc
      // Player player1;
      // Player player2;
      // getPlayer(player1Type, &player1);
      // getPlayer(player2Type, &player2);
      GameState gameState;
      play(&gameState, &player1, &player2);
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

void getPlayer(std::string playerType, Player* player) {
  if (playerType == "random") {
    RandomPlayer initPlayer;
    *player = initPlayer;
  } else if (playerType == "opt") {
    OptimalPlayer initPlayer;
    *player = initPlayer;
  } else if (playerType == "mcts") {
    MCTSPlayer initPlayer;
    *player = initPlayer;
  } else {
    std::cerr << "error: " << "unknown player type: " << playerType << "\n";
    exit(1);
  }
}
