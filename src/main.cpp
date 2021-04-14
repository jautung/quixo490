#include "game/GamePlayHandler.hpp"
#include "game/GameStateHandler.hpp"
#include "game/GraphicsHandler.hpp"
#include "optimal/OptAnalyzer.hpp"
#include "optimal/OptComputer.hpp"
#include "players/Players.hpp"
#include "utils/CliHandler.hpp"
#include <chrono>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <random>
#include <string>
#include <tclap/CmdLine.h>
#include <vector>

std::mt19937 rng(time(0));

Player* getPlayer(std::string playerType, GameStateHandler* gameStateHandler, CliHandler* cliHandler, GraphicsHandler* graphicsHandler = NULL);
state_t getStateInteractive(GraphicsHandler* graphicsHandler);

int main(int argc, char* argv[]) {
  try {
    TCLAP::CmdLine cmd("Quixo Project");

    TCLAP::ValueArg<std::string> progArg("p", "program", "Program to run (`play`, `test`, `test-move-correctness`, `opt-compute*`, `opt-check`, `opt-analyze`, or `opt-analyze-adj`)", false, "play", "string", cmd);
    TCLAP::ValueArg<int> lenArg("l", "len", "For `play`, `test`, `test-move-correctness`, `opt-compute`, `opt-check`, `opt-analyze` or `opt-analyze-adj` program: number of tiles per side", false, 5, "integer", cmd);
    TCLAP::ValueArg<std::string> playerXTypeArg("X", "playerX", "For `play`, `test` or `test-move-correctness` program: player X type (`random`, `interact`, `opt*`, `heuris-simple`, `mcts*,*`, `mcts-cache-persist*,*`, or `q-learn*,*`)", false, "random", "string", cmd);
    TCLAP::ValueArg<std::string> playerOTypeArg("O", "playerO", "For `play` or `test` program: player O type (`random`, `interact`, `opt*`, `heuris-simple`, `mcts*,*`, `mcts-cache-persist*,*`, or `q-learn*,*`)", false, "random", "string", cmd);
    TCLAP::ValueArg<int> nTurnsArg("n", "numturns", "For `play` or `test` program: turn limit per game (<=0: till the end)", false, 0, "integer", cmd);
    TCLAP::SwitchArg initStateArg("i", "initstate", "For `play` program: whether to set an initial state of the game board", cmd);
    TCLAP::ValueArg<int> timePauseMsArg("t", "timepause", "For `play` program: time (in milliseconds) to pause between steps", false, 0, "integer", cmd);
    TCLAP::ValueArg<int> graphicsResArg("g", "graphicsres", "For `play` or `opt-check` program: graphical output screen resolution (<0: no graphics)", false, 0, "integer", cmd);
    TCLAP::ValueArg<int> numGamesArg("N", "Ngames", "For `test` or `test-move-correctness` program: number of game iterations or game states (respectively) to run", false, 1, "integer", cmd);
    TCLAP::ValueArg<int> numThreadsArg("T", "Threads", "For `opt-compute` program: number of Threads to use", false, 1, "integer", cmd);
    TCLAP::ValueArg<int> numLocksPerArrArg("L", "Locksperarray", "For `opt-compute` program: number of Locks to use per result array", false, 1, "integer", cmd);
    TCLAP::ValueArg<std::string> verbosityArg("v", "verbosity", "For `play`, `test` or `test-move-correctness` program: verbosity of logging (`all`, `default`, `error-rate-tests-*`, `game-length-tests-*`, `correct-move-tests`, or `silent`)", false, "default", "string", cmd);
    cmd.parse(argc, argv);

    auto prog = progArg.getValue();
    auto len = lenArg.getValue();
    if (len <= 1 || len >= 6) {
      std::cerr << "warning: " << "len=" << len << " is out of range and will be automatically reverted to default len=5\n";
      len = 5;
    }
    auto playerXType = playerXTypeArg.getValue();
    auto playerOType = playerOTypeArg.getValue();
    auto nTurns = nTurnsArg.getValue();
    auto initState = initStateArg.getValue();
    auto timePauseMs = timePauseMsArg.getValue();
    auto graphicsRes = graphicsResArg.getValue();
    auto numGames = numGamesArg.getValue();
    auto numThreads = numThreadsArg.getValue();
    if (numThreads <= 0) {
      std::cerr << "warning: " << "number of threads requested (" << numThreads << ") is not positive; automatically reverting to default of 1 thread\n";
      numThreads = 1;
    } else if (numThreads > 16) {
      std::cerr << "warning: " << "number of threads requested (" << numThreads << ") is larger than 16; automatically reverting to default of 1 thread\n";
      numThreads = 1;
    } else if (numThreads > omp_get_num_procs()) {
      std::cerr << "warning: " << "number of threads requested (" << numThreads << ") is larger than the number of processors available (" << omp_get_num_procs() << "); proceed with caution\n";
    }
    auto numLocksPerArr = numLocksPerArrArg.getValue();
    if (numLocksPerArr <= 0) {
      std::cerr << "warning: " << "number of locks per array requested (" << numLocksPerArr << ") is not positive; automatically reverting to default of 1 lock per array\n";
      numLocksPerArr = 1;
    }
    auto verbosity = verbosityArg.getValue();
    if (verbosity != "all" && verbosity != "default" && verbosity != "error-rate-tests-mcts" && verbosity != "error-rate-tests-q-learn" && verbosity != "game-length-tests-mcts" && verbosity != "game-length-tests-q-learn" && verbosity != "correct-move-tests" && verbosity != "silent") {
      std::cerr << "error: " << "unknown verbosity: " << verbosity << "\n";
      exit(1);
    }

    auto gameStateHandler = new GameStateHandler(len);

    if (prog == "play") {
      auto cliHandler = new CliHandler();
      auto graphicsHandler = graphicsRes > 0 ? new GraphicsHandler(gameStateHandler, graphicsRes) : NULL;
      auto playerX = getPlayer(playerXType, gameStateHandler, cliHandler, graphicsHandler);
      auto playerO = getPlayer(playerOType, gameStateHandler, cliHandler, graphicsHandler);
      auto gamePlayHandler = new GamePlayHandler(playerX, playerO, timePauseMs, gameStateHandler, graphicsHandler, verbosity);
      gamePlayHandler->startGame(initState ? getStateInteractive(graphicsHandler) : 0b0);
      int nTurnsPlayed;
      auto winner = nTurns <= 0 ? gamePlayHandler->playTillEnd(nTurnsPlayed) : gamePlayHandler->playNTurns(nTurns, nTurnsPlayed);
      if (winner == WINNER_X) {
        if (verbosity == "all" || verbosity == "default") std::cout << "X Wins (after " << nTurnsPlayed << " turns)!\n";
      } else if (winner == WINNER_O) {
        if (verbosity == "all" || verbosity == "default") std::cout << "O Wins (after " << nTurnsPlayed << " turns)!\n";
      } else if (winner == WINNER_UNKNOWN) {
        if (verbosity == "all" || verbosity == "default") std::cout << "No Winner Yet (after " << nTurnsPlayed << " turns).\n";
      }
      #if MCTS_CACHE_HIT_CHECK == 1
        if (playerXType.find("mcts", 0) == 0) static_cast<MCTSPlayer*>(playerX)->printCacheStats("X");
        if (playerOType.find("mcts", 0) == 0) static_cast<MCTSPlayer*>(playerO)->printCacheStats("O");
      #endif
      delete cliHandler;
      delete graphicsHandler;
      delete playerX;
      delete playerO;
      delete gamePlayHandler;
    }

    else if (prog == "test") {
      if (playerXType == "interact" || playerOType == "interact") {
        std::cerr << "warning: " << "using an interactive player for test runs is not a good idea; aborting\n";
        exit(1);
      }
      auto cliHandler = new CliHandler();
      auto playerX = getPlayer(playerXType, gameStateHandler, cliHandler);
      auto playerO = getPlayer(playerOType, gameStateHandler, cliHandler);
      auto gamePlayHandler = new GamePlayHandler(playerX, playerO, 0, gameStateHandler, NULL, verbosity);
      int xWins = 0;
      int oWins = 0;
      int draws = 0;
      std::vector<int> numTurns;
      for (int i = 0; i < numGames; i++) {
        gamePlayHandler->startGame();
        int nTurnsPlayed;
        auto winner = nTurns <= 0 ? gamePlayHandler->playTillEnd(nTurnsPlayed) : gamePlayHandler->playNTurns(nTurns, nTurnsPlayed);
        if (winner == WINNER_X) {
          xWins += 1;
          if (verbosity == "all" || verbosity == "default") std::cout << i << ": X Wins (after " << nTurnsPlayed << " turns)!\n";
        } else if (winner == WINNER_O) {
          oWins += 1;
          if (verbosity == "all" || verbosity == "default") std::cout << i << ": O Wins (after " << nTurnsPlayed << " turns)!\n";
        } else if (winner == WINNER_UNKNOWN) {
          draws += 1;
          if (verbosity == "all" || verbosity == "default") std::cout << i << ": Draw (after " << nTurnsPlayed << " turns)!\n";
        }
        numTurns.push_back(nTurnsPlayed);
      }
      int totalNumTurns = std::accumulate(numTurns.begin(), numTurns.end(), 0);
      if (verbosity == "all" || verbosity == "default") {
        std::cout << "\nResult summary for Player X (" << playerXType << ") vs. Player O (" << playerOType << ") on " << len << "X" << len << " Quixo\n";
        std::cout << "--------------------------------------------------------------------------------\n";
        if (playerXType.find("opt", 0) != 0) std::cout << "Player X init compute time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->initTimeX).count()/1000.0 << " (" << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->initTimeX).count()/1000.0/numGames << " average per game)\n";
        if (playerOType.find("opt", 0) != 0) std::cout << "Player O init compute time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->initTimeO).count()/1000.0 << " (" << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->initTimeO).count()/1000.0/numGames << " average per game)\n";
        std::cout << "Player X running compute time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeX).count()/1000.0 << " (" << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeX).count()/1000.0/numGames << " average per game; " << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeX).count()/1000.0/totalNumTurns << " average per turn)\n";
        std::cout << "Player O running compute time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeO).count()/1000.0 << " (" << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeO).count()/1000.0/numGames << " average per game; " << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeO).count()/1000.0/totalNumTurns << " average per turn)\n";
        std::cout << "Winners (X-O-D): " << xWins << "-" << oWins << "-" << draws << "\n";
      } else if (verbosity == "error-rate-tests-mcts" || verbosity == "error-rate-tests-q-learn") {
        std::cout << len << "\t" << playerXType << "\t" << playerOType << "\t"
                  << xWins << "\t" << oWins << "\t" << draws << "\t";
        if (verbosity == "error-rate-tests-mcts") {
          std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeX).count()/1000.0/totalNumTurns << "\t"
                    << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeO).count()/1000.0/totalNumTurns << "\n";
        } else if (verbosity == "error-rate-tests-q-learn") {
          std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeX).count()/1000.0/numGames << "\t"
                    << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeO).count()/1000.0/numGames << "\n";
        }
      } else if (verbosity == "game-length-tests-mcts" || verbosity == "game-length-tests-q-learn") {
        double aveNumTurns = 1.0*totalNumTurns/numGames;
        double stdevAccum = 0.0;
        std::for_each(numTurns.begin(), numTurns.end(), [&](const double numTurn) {
          stdevAccum += (numTurn - aveNumTurns) * (numTurn - aveNumTurns);
        });
        double stdevNumTurns = sqrt(stdevAccum/(numGames-1));
        std::cout << len << "\t" << playerXType << "\t" << playerOType << "\t"
                  << aveNumTurns << "\t" << stdevNumTurns << "\t" << draws << "\t";
        if (verbosity == "game-length-tests-mcts") {
          std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeX).count()/1000.0/totalNumTurns << "\t"
                    << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeO).count()/1000.0/totalNumTurns << "\n";
        } else if (verbosity == "game-length-tests-q-learn") {
          std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeX).count()/1000.0/numGames << "\t"
                    << std::chrono::duration_cast<std::chrono::milliseconds>(gamePlayHandler->runTimeO).count()/1000.0/numGames << "\n";
        }
      }
      delete cliHandler;
      delete playerX;
      delete playerO;
      delete gamePlayHandler;
    }

    else if (prog == "test-move-correctness") {
      if (playerXType == "interact") {
        std::cerr << "warning: " << "using an interactive player for testing move correctness is not a good idea; aborting\n";
        exit(1);
      }
      auto cliHandler = new CliHandler();
      auto playerX = getPlayer(playerXType, gameStateHandler, cliHandler);
      auto optimalPlayer = new OptimalPlayer(gameStateHandler);
      int numCorrectMovesFromLossState = 0, numCorrectMovesFromDrawState = 0, numCorrectMovesFromWinState = 0, numLossStates = 0, numDrawStates = 0, numWinStates = 0;
      for (int i = 0; i < numGames; i++) {
        playerX->clearCache();
        playerX->initLearn();
        auto state = gameStateHandler->genRandomNonTerminalState();
        auto result = optimalPlayer->evalState(state);
        auto move = playerX->selectMove(state);
        auto nextState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
        auto nextResult = optimalPlayer->evalState(nextState);
        std::string toPrint;
        if (result == RESULT_LOSS) {
          numLossStates += 1;
          toPrint = "Correct move for LOSS state";
          numCorrectMovesFromLossState += 1;
        } else if (result == RESULT_DRAW) {
          numDrawStates += 1;
          if (nextResult == RESULT_DRAW) {
            toPrint = "Correct move for DRAW state";
            numCorrectMovesFromDrawState += 1;
          } else {
            toPrint = "Incorrect move for DRAW state";
          }
        } else if (result == RESULT_WIN) {
          numWinStates += 1;
          if (nextResult == RESULT_LOSS) {
            toPrint = "Correct move for WIN state";
            numCorrectMovesFromWinState += 1;
          } else {
            toPrint = "Incorrect move for WIN state";
          }
        }
        if (verbosity == "all" || verbosity == "default") std::cout << "Game state " << i << ": ";
        if (verbosity == "all") {
          std::cout << "\n";
          gameStateHandler->print(state);
        }
        if (verbosity == "all" || verbosity == "default") std::cout << toPrint << "\n";
        if (verbosity == "all") std::cout << "\n";
      }
      int numCorrectMoves = numCorrectMovesFromLossState + numCorrectMovesFromDrawState + numCorrectMovesFromWinState;
      if (verbosity == "all" || verbosity == "default") {
        std::cout << "\nResult summary for Player X (" << playerXType << ") on " << len << "X" << len << " Quixo\n";
        std::cout << "--------------------------------------------------------------------------------\n";
        std::cout << "Number of states attempted: " << numGames << " (" << numLossStates << " L / " << numDrawStates << " D / " << numWinStates << " W)\n";
        std::cout << "Number of correct moves: " << numCorrectMoves << " (" << numCorrectMovesFromLossState << " L / " << numCorrectMovesFromDrawState << " D / " << numCorrectMovesFromWinState << " W)\n";
        std::cout << "Correct move percentage: " << 100.0*numCorrectMoves/numGames << "% (" << 100.0*numCorrectMovesFromLossState/numLossStates << "% L / " << 100.0*numCorrectMovesFromDrawState/numDrawStates << "% D / " << 100.0*numCorrectMovesFromWinState/numWinStates << "% W)\n";
      } else if (verbosity == "correct-move-tests") {
        std::cout << len << "\t" << playerXType << "\t"
                  << 1.0*numCorrectMoves/numGames << "\t"
                  << 1.0*numCorrectMovesFromLossState/numLossStates << "\t"
                  << 1.0*numCorrectMovesFromDrawState/numDrawStates << "\t"
                  << 1.0*numCorrectMovesFromWinState/numWinStates << "\n";
      }
      delete cliHandler;
      delete playerX;
      delete optimalPlayer;
    }

    else if (prog.find("opt-compute", 0) == 0) { // string starts with opt-compute prefix
      auto cliHandler = new CliHandler();
      auto cliParams = cliHandler->readCliParams(prog.substr(std::string("opt-compute").length()));
      if (cliParams.size() != 0 && cliParams.size() != 1) {
        std::cerr << "error: " << "incorrect number of params for opt-compute program (expected 0 or 1; got " << cliParams.size() << ")\n";
        exit(1);
      }
      nbit_t numUsedComputeTill = 0;
      if (cliParams.size() == 1 && cliParams[0] != "") {
        numUsedComputeTill = cliHandler->readNonNegIntCliParam(cliParams[0], "opt-compute numUsedComputeTill");
      }
      auto startTime = std::chrono::high_resolution_clock::now();
      auto optComputer = new OptComputer(gameStateHandler, numThreads, numLocksPerArr);
      auto endTime = std::chrono::high_resolution_clock::now();
      std::cout << "OptComputer initialization time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count()/1000.0 << "\n";
      optComputer->computeAll(numUsedComputeTill);
      endTime = std::chrono::high_resolution_clock::now();
      std::cout << "OptComputer total time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count()/1000.0 << "\n";
      std::cout << "Total file I/O time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(optComputer->dataHandler->ioTime).count()/1000.0 << "\n";
      delete cliHandler;
      delete optComputer;
    }

    else if (prog == "opt-check") {
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
    }

    else if (prog == "opt-analyze") {
      auto optAnalyzer = new OptAnalyzer(gameStateHandler);
      optAnalyzer->analyzeNumWinLossDrawStates(); // full counts of win, loss, and draw states
      delete optAnalyzer;
    }

    else if (prog == "opt-analyze-adj") {
      auto optAnalyzer = new OptAnalyzer(gameStateHandler);
      optAnalyzer->analyzeNumWinLossDrawStates(true); // counts of win, loss, and draw states for adjacent numbers of Xs and Os (what the paper does)
      delete optAnalyzer;
    }

    else {
      std::cerr << "error: " << "unknown program: " << prog << "\n";
      exit(1);
    }
    delete gameStateHandler;

  } catch (TCLAP::ArgException const &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << "\n";
  }

  return 0;
}

Player* getPlayer(std::string playerType, GameStateHandler* gameStateHandler, CliHandler* cliHandler, GraphicsHandler* graphicsHandler) {
  if (playerType == "random") {
    return new RandomPlayer(gameStateHandler);
  } else if (playerType == "interact") {
    return new InteractivePlayer(gameStateHandler, graphicsHandler);
  } else if (playerType.find("opt", 0) == 0) {
    auto cliParams = cliHandler->readCliParams(playerType.substr(std::string("opt").length()));
    if (cliParams.size() != 0 && cliParams.size() != 1) {
      std::cerr << "error: " << "incorrect number of params for opt player type (expected 0 or 1; got " << cliParams.size() << ")\n";
      exit(1);
    }
    double initErrorRate = 0.0;
    if (cliParams.size() == 1 && cliParams[0] != "") {
      initErrorRate = cliHandler->readFracDoubleCliParam(cliParams[0], "opt initErrorRate");
    }
    return new OptimalPlayer(gameStateHandler, graphicsHandler, initErrorRate);
  } else if (playerType == "heuris-simple") {
    return new HeuristicSimplePlayer(gameStateHandler);
  } else if (playerType.find("mcts", 0) == 0) { // string starts with mcts prefix
    std::string prefix = "mcts";
    bool persistCacheQ = false;
    if (playerType.find("mcts-cache-persist", 0) == 0) { // string starts with mcts-cache-persist prefix
      prefix = "mcts-cache-persist";
      persistCacheQ = true;
    }
    auto cliParams = cliHandler->readCliParams(playerType.substr(std::string(prefix).length()));
    if (cliParams.size() != 2) {
      std::cerr << "error: " << "incorrect number of params for mcts player type (expected 2; got " << cliParams.size() << ")\n";
      exit(1);
    }
    auto initIters = cliHandler->readNonNegIntCliParam(cliParams[0], "mcts player init iters");
    auto perMoveIters = cliHandler->readNonNegIntCliParam(cliParams[1], "mcts player per move iters");
    return new MCTSPlayer(gameStateHandler, graphicsHandler, initIters, perMoveIters, persistCacheQ);
  } else if (playerType.find("q-learn", 0) == 0) { // string starts with q-learn prefix
    auto cliParams = cliHandler->readCliParams(playerType.substr(std::string("q-learn").length()));
    if (cliParams.size() != 2) {
      std::cerr << "error: " << "incorrect number of params for q-learning player type (expected 2; got " << cliParams.size() << ")\n";
      exit(1);
    }
    auto initIters = cliHandler->readNonNegIntCliParam(cliParams[0], "q-learning player init iters");
    auto perMoveIters = cliHandler->readNonNegIntCliParam(cliParams[1], "q-learning player per move iters");
    return new QLearningPlayer(gameStateHandler, graphicsHandler, initIters, perMoveIters);
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
