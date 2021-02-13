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
  try {
    TCLAP::CmdLine cmd("Quixo Project");
    TCLAP::ValueArg<std::string> progArg("p", "program", "Program to run (`play`, `opt-compute`, or `mcts-compute`)", false, "play", "string", cmd);
    TCLAP::ValueArg<std::string> playerXTypeArg("X", "playerX", "For `play` program: player X type (`random`, `opt`, or `mcts`)", false, "random", "string", cmd);
    TCLAP::ValueArg<std::string> playerOTypeArg("O", "playerO", "For `play` program: player O type (`random`, `opt`, or `mcts`)", false, "random", "string", cmd);
    TCLAP::ValueArg<int> nStepsArg("n", "nsteps", "For `play` program: number of steps to run (0: till the end)", false, 0, "integer", cmd);
    TCLAP::ValueArg<int> timeStepMsArg("s", "timestep", "For `play` program: time (in milliseconds) to pause between steps", false, 0, "integer", cmd);
    TCLAP::SwitchArg graphicsArg("g", "graphics", "For `play` program: graphical output", cmd, false);
    cmd.parse(argc, argv);
    auto prog = progArg.getValue();
    auto playerXType = playerXTypeArg.getValue();
    auto playerOType = playerOTypeArg.getValue();
    auto nSteps = nStepsArg.getValue();
    auto timeStepMs = timeStepMsArg.getValue();
    auto graphics = graphicsArg.getValue();

    if (prog == "play") {
      auto now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());
      srand(now.time_since_epoch().count());
      auto playerX = getPlayer(playerXType);
      auto playerO = getPlayer(playerOType);
      auto gameState = new GameState();
      auto gamePlay = new GamePlay(gameState, playerX, playerO, timeStepMs, graphics);
      auto winner = nSteps == 0 ? gamePlay->playTillEnd() : gamePlay->playNTurns(nSteps);
      if (winner == WINNER_X) {
        std::cout << "X Wins!\n";
      } else if (winner == WINNER_O) {
        std::cout << "O Wins!\n";
      } else {
        std::cout << "No Winner Yet.\n";
      }
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
