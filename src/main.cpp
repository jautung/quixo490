#include "game/GamePlayHandler.hpp"
#include "game/GameStateHandler.hpp"
#include "game/GraphicsHandler.hpp"
#include "optimal/OptComputer.hpp"
#include "players/Players.hpp"
#include "utils/MemoryChecker.hpp"
#include <chrono>
#include <iostream>
#include <omp.h>
#include <random>
#include <tclap/CmdLine.h>

Player* getPlayer(std::string playerType, GameStateHandler* gameStateHandler, GraphicsHandler* graphicsHandler = NULL) {
  if (playerType == "random") {
    return new RandomPlayer(gameStateHandler);
  } else if (playerType == "interact") {
    return new InteractivePlayer(gameStateHandler, graphicsHandler);
  } else if (playerType == "opt") {
    return new OptimalPlayer(gameStateHandler);
  } else if (playerType == "heuris-simple") {
    return new HeuristicSimplePlayer(gameStateHandler);
  } else if (playerType.rfind("mcts", 0) == 0) { // string starts with mcts prefix
    auto itersStr = playerType.substr(4);
    try {
      size_t stoiIndex;
      auto iters = std::stoi(itersStr, &stoiIndex);
      if (stoiIndex != itersStr.length()) {
        std::cerr << "error: " << "non-integer mcts iters: " << itersStr << "\n";
        exit(1);
      }
      if (iters < 0) {
        std::cerr << "error: " << "negative mcts iters: " << itersStr << "\n";
        exit(1);
      }
      return new MCTSPlayer(gameStateHandler, graphicsHandler, iters);
    } catch (std::exception const &e) {
      std::cerr << "error: " << "non-integer mcts iters: " << itersStr << "\n";
      exit(1);
    }
  } else {
    std::cerr << "error: " << "unknown player type: " << playerType << "\n";
    exit(1);
  }
}

state_t getStateInteractive(GraphicsHandler* graphicsHandler) {
  if (graphicsHandler) {
    std::cout << "Remember to hit Enter to confirm your selection\n";
    auto state = graphicsHandler->drawBaseBoardGetState();
    return state;
  } else {
    state_t state;
    std::cerr << "warning: " << "running optimal checker without a graphics handler is EXTREMELY unadvised; proceed only if you know EXACTLY how states are indexed\n";
    std::cout << "Choose a state index (no error checking; proceed at your own peril): ";
    std::cin >> state;
    return state;
  }
}

int main(int argc, char* argv[]) {
  try {
    TCLAP::CmdLine cmd("Quixo Project");
    TCLAP::ValueArg<std::string> progArg("p", "program", "Program to run (`play`, `test`, `opt-compute`, `opt-check`)", false, "play", "string", cmd);
    TCLAP::ValueArg<int> lenArg("l", "len", "For `play`, `test`, `opt-compute` or `opt-check` program: number of tiles per side", false, 5, "integer", cmd);
    TCLAP::ValueArg<std::string> playerXTypeArg("X", "playerX", "For `play` or `test` program: player X type (`random`, `interact`, `opt`, `heuris-simple`, or `mcts`)", false, "random", "string", cmd);
    TCLAP::ValueArg<std::string> playerOTypeArg("O", "playerO", "For `play` or `test` program: player O type (`random`, `interact`, `opt`, `heuris-simple`, or `mcts`)", false, "random", "string", cmd);
    TCLAP::ValueArg<int> numStepsArg("n", "numsteps", "For `play` or `test` program: number of steps to run per game (<=0: till the end)", false, 0, "integer", cmd);
    TCLAP::SwitchArg initStateArg("i", "initstate", "For `play` program: whether to set an initial state of the game board", cmd);
    TCLAP::ValueArg<int> timePauseMsArg("t", "timepause", "For `play` program: time (in milliseconds) to pause between steps", false, 0, "integer", cmd);
    TCLAP::ValueArg<int> graphicsResArg("g", "graphicsres", "For `play` or `opt-check` program: graphical output screen resolution (<0: no graphics)", false, 0, "integer", cmd);
    TCLAP::ValueArg<int> numGamesArg("N", "Ngames", "For `test` program: number of game iterations to run", false, 1, "integer", cmd);
    TCLAP::SwitchArg memoryCheckArg("m", "memorycheck", "For `opt-compute` program: check run-time memory usage", cmd);
    TCLAP::ValueArg<int> numThreadsArg("T", "Threads", "For `opt-compute` program: number of Threads to use", false, 1, "integer", cmd);
    cmd.parse(argc, argv);
    auto prog = progArg.getValue();
    auto len = lenArg.getValue();
    if (len <= 1 || len >= 6) {
      std::cerr << "warning: " << "len=" << len << " is out of range and will be automatically reverted to default len=5\n";
      len = 5;
    }
    auto playerXType = playerXTypeArg.getValue();
    auto playerOType = playerOTypeArg.getValue();
    auto numSteps = numStepsArg.getValue();
    auto initState = initStateArg.getValue();
    auto timePauseMs = timePauseMsArg.getValue();
    auto graphicsRes = graphicsResArg.getValue();
    auto numGames = numGamesArg.getValue();
    auto memoryCheck = memoryCheckArg.getValue();
    auto numThreads = numThreadsArg.getValue();

    auto gameStateHandler = new GameStateHandler(len);
    if (prog == "play") {
      srand(time(0));
      auto graphicsHandler = graphicsRes > 0 ? new GraphicsHandler(gameStateHandler, graphicsRes) : NULL;
      auto playerX = getPlayer(playerXType, gameStateHandler, graphicsHandler);
      auto playerO = getPlayer(playerOType, gameStateHandler, graphicsHandler);
      auto gamePlayHandler = new GamePlayHandler(playerX, playerO, timePauseMs, gameStateHandler, graphicsHandler);
      gamePlayHandler->startGame(initState ? getStateInteractive(graphicsHandler) : 0b0);
      auto winner = numSteps <= 0 ? gamePlayHandler->playTillEnd() : gamePlayHandler->playNTurns(numSteps);
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
      for (int i = 0; i < numGames; i++) {
        gamePlayHandler->startGame();
        auto winner = numSteps <= 0 ? gamePlayHandler->playTillEnd() : gamePlayHandler->playNTurns(numSteps);
        if (winner == WINNER_X) {
          xWins += 1;
          std::cout << i << ": X Wins!\n";
        } else if (winner == WINNER_O) {
          oWins += 1;
          std::cout << i << ": O Wins!\n";
        } else if (winner == WINNER_UNKNOWN) {
          draws += 1;
          std::cout << i << ": Draw!\n";
        }
      }
      std::cout << "Results (X-O-D): " << xWins << "-" << oWins << "-" << draws << "\n";
      delete playerX;
      delete playerO;
      delete gamePlayHandler;
    } else if (prog == "opt-compute") {
      auto memoryChecker = memoryCheck ? new MemoryChecker() : NULL;
      if (numThreads <= 0) {
        std::cerr << "warning: " << "number of threads requested (" << numThreads << ") is not positive; automatically reverting to default of 1 thread\n";
        numThreads = 1;
      } else if (numThreads > omp_get_num_procs()) {
        std::cerr << "warning: " << "number of threads requested (" << numThreads << ") is larger than the number of processors available (" << omp_get_num_procs() << "); proceed with caution\n";
      }
      omp_set_num_threads(numThreads);
      auto startTime = std::chrono::high_resolution_clock::now();
      auto optComputer = new OptComputer(len*len, gameStateHandler, memoryChecker);
      auto endTime = std::chrono::high_resolution_clock::now();
      std::cout << "OptComputer initialization time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count()/1000.0 << "\n";
      optComputer->computeAll();
      endTime = std::chrono::high_resolution_clock::now();
      std::cout << "OptComputer total time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count()/1000.0 << "\n";
      std::cout << "Total file I/O time (s): " << optComputer->dataHandler->ioTime/1000.0 << "\n";
      delete memoryChecker;
      delete optComputer;
    } else if (prog == "opt-check") {
      auto graphicsHandler = graphicsRes > 0 ? new GraphicsHandler(gameStateHandler, graphicsRes) : NULL;
      auto state = getStateInteractive(graphicsHandler);
      auto optimalPlayer = new OptimalPlayer(gameStateHandler);
      auto result = optimalPlayer->evalState(state);
      if (result == RESULT_WIN) {
        std::cout << "Win state\n";
      } else if (result == RESULT_LOSS) {
        std::cout << "Loss state\n";
      } else if (result == RESULT_DRAW) {
        std::cout << "Draw state\n";
      }
      delete graphicsHandler;
      delete optimalPlayer;
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
