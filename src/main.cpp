#include "game/GamePlayHandler.hpp"
#include "game/GameStateHandler.hpp"
#include "game/GraphicsHandler.hpp"
#include "optimal/OptComputer.hpp"
#include "players/Players.hpp"
#include <iostream>
#include <random>
#include <tclap/CmdLine.h>

Player* getPlayer(std::string playerType, GameStateHandler* gameStateHandler, GraphicsHandler* graphicsHandler = NULL) {
  if (playerType == "random") {
    return new RandomPlayer(gameStateHandler);
  } else if (playerType == "interact") {
    return new InteractivePlayer(gameStateHandler, graphicsHandler);
  } else if (playerType == "opt") {
    return new OptimalPlayer(gameStateHandler);
  } else if (playerType == "mcts") {
    return new MCTSPlayer(gameStateHandler);
  } else {
    std::cerr << "error: " << "unknown player type: " << playerType << "\n";
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  try {
    TCLAP::CmdLine cmd("Quixo Project");
    TCLAP::ValueArg<std::string> progArg("p", "program", "Program to run (`play`, `test`, `opt-compute`, or `mcts-compute`)", false, "play", "string", cmd);
    TCLAP::ValueArg<int> lenArg("l", "len", "For `play`, `test` or `opt-compute` program: number of tiles per side", false, 5, "integer", cmd);
    TCLAP::ValueArg<std::string> playerXTypeArg("X", "playerX", "For `play` or `test` program: player X type (`random`, `interact`, `opt`, or `mcts`)", false, "random", "string", cmd);
    TCLAP::ValueArg<std::string> playerOTypeArg("O", "playerO", "For `play` or `test` program: player O type (`random`, `interact`, `opt`, or `mcts`)", false, "random", "string", cmd);
    TCLAP::ValueArg<int> nStepsArg("n", "nsteps", "For `play` or `test` program: number of steps or iterations to run respectively (0: till the end for `play`)", false, 0, "integer", cmd);
    TCLAP::ValueArg<int> timePauseMsArg("t", "timepause", "For `play` program: time (in milliseconds) to pause between steps", false, 0, "integer", cmd);
    TCLAP::ValueArg<int> graphicsResArg("g", "graphicsres", "For `play` program: graphical output screen resolution (0: no graphics)", false, 0, "integer", cmd);
    cmd.parse(argc, argv);
    auto prog = progArg.getValue();
    auto len = lenArg.getValue();
    if (len <= 1 || len >= 6) {
      std::cerr << "warning: " << "len=" << len << " is out of range and will be automatically reverted to default len=5\n";
      len = 5;
    }
    auto playerXType = playerXTypeArg.getValue();
    auto playerOType = playerOTypeArg.getValue();
    auto nSteps = nStepsArg.getValue();
    auto timePauseMs = timePauseMsArg.getValue();
    auto graphicsRes = graphicsResArg.getValue();

    auto gameStateHandler = new GameStateHandler(len);
    if (prog == "play") {
      srand(time(0));
      auto graphicsHandler = graphicsRes > 0 ? new GraphicsHandler(graphicsRes, gameStateHandler) : NULL;
      auto playerX = getPlayer(playerXType, gameStateHandler, graphicsHandler);
      auto playerO = getPlayer(playerOType, gameStateHandler, graphicsHandler);
      auto gamePlayHandler = new GamePlayHandler(playerX, playerO, timePauseMs, gameStateHandler, graphicsHandler);
      gamePlayHandler->startGame();
      auto winner = nSteps <= 0 ? gamePlayHandler->playTillEnd() : gamePlayHandler->playNTurns(nSteps);
      if (winner == WINNER_X) {
        std::cout << "X Wins!\n";
      } else if (winner == WINNER_O) {
        std::cout << "O Wins!\n";
      } else if (winner == WINNER_UNKNOWN) {
        std::cout << "No Winner Yet.\n";
      }
      delete graphicsHandler;
      delete playerX;
      delete playerO;
      delete gamePlayHandler;
    } else if (prog == "test") {
      srand(time(0));
      if (playerXType == "interact" || playerOType == "interact") {
        std::cerr << "warning: " << "using an interactive player for test runs is not a good idea; aborting\n";
        exit(1);
      }
      auto playerX = getPlayer(playerXType, gameStateHandler);
      auto playerO = getPlayer(playerOType, gameStateHandler);
      auto gamePlayHandler = new GamePlayHandler(playerX, playerO, timePauseMs, gameStateHandler, NULL, true);
      int xWins = 0;
      int oWins = 0;
      int draws = 0;
      for (int i = 0; i < nSteps; i++) {
        gamePlayHandler->startGame();
        auto winner = gamePlayHandler->playTillEnd();
        if (winner == WINNER_X) {
          xWins += 1;
        } else if (winner == WINNER_O) {
          oWins += 1;
        } else if (winner == WINNER_UNKNOWN) {
          draws += 1;
        }
      }
      std::cout << "Results (X-O-D): " << xWins << "-" << oWins << "-" << draws << "\n";
      delete playerX;
      delete playerO;
      delete gamePlayHandler;
    } else if (prog == "opt-compute") {
      auto optComputer = new OptComputer(len*len, gameStateHandler);
      optComputer->computeAll();
      delete optComputer;
    } else if (prog == "mcts-compute") {
      // TODO
    } else {
      std::cerr << "error: " << "unknown program: " << prog << "\n";
      exit(1);
    }
    delete gameStateHandler;
  } catch (TCLAP::ArgException const &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << "\n";
  }
  return 0;
}
