#include "../game/GameStateHandler.hpp"
#include "../optimal/OptComputer.hpp"
#include "../utils/DataHandler.hpp"
#include "OptAnalyzer.hpp"
#include <inttypes.h>
#include <unordered_map>

namespace {
  byte_t resultMask = 0b11;
}

OptAnalyzer::OptAnalyzer(GameStateHandler* initGameStateHandler) {
  gameStateHandler = initGameStateHandler;
  len = gameStateHandler->len;
  optComputer = new OptComputer(gameStateHandler);
  dataHandler = new DataHandler();
}

OptAnalyzer::~OptAnalyzer() {
  delete optComputer;
  delete dataHandler;
}

void OptAnalyzer::analyzeNumWinLossDrawStates(bool adjacentXOs) {
  printf("(X,O)\tWins\tLosses\tDraws\tTotal\t%% Wins\t%% Losses\t%% Draws\n");
  for (nbit_t numX = 0; numX <= len*len; numX++) {
    for (nbit_t numO = 0; numO <= len*len - numX; numO++) {
      if (adjacentXOs && (numO - numX > 1 || numO < numX)) continue;
      auto results = dataHandler->loadClass(len, numX, numO);
      sindex_t wins=0, losses=0, draws=0;
      sindex_t numStates = optComputer->numStatesClass(numX, numO);
      for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
        auto result = dataHandler->getResult(results, stateIndex);
        if (result == RESULT_WIN) wins++;
        if (result == RESULT_LOSS) losses++;
        if (result == RESULT_DRAW) draws++;
      }
      auto total = wins + losses + draws;
      printf("(%d,%d)\t%" PRId64 "\t%" PRId64 "\t%" PRId64 "\t%" PRId64 "\t%.10f\t%.10f\t%.10f\n", numX, numO, wins, losses, draws, total, 1.0*wins/total, 1.0*losses/total, 1.0*draws/total);
    }
  }
}

void OptAnalyzer::analyzeResultsSteps() {
  std::unordered_map<nsteps_t, sindex_t> winsPerResultStep;
  std::unordered_map<nsteps_t, sindex_t> lossesPerResultStep;
  for (nbit_t numX = 0; numX <= len*len; numX++) {
    for (nbit_t numO = 0; numO <= len*len - numX; numO++) {
      auto results = dataHandler->loadClass(len, numX, numO);
      auto resultsSteps = dataHandler->loadClassSteps(len, numX, numO);
      sindex_t numStates = optComputer->numStatesClass(numX, numO);
      for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
        auto result = dataHandler->getResult(results, stateIndex);
        auto resultStep = dataHandler->getResultStep(resultsSteps, stateIndex);
        if (result == RESULT_WIN) winsPerResultStep[resultStep]++;
        if (result == RESULT_LOSS) lossesPerResultStep[resultStep]++;
      }
    }
  }
  printf("Steps\tWins\tLosses\n");
  sindex_t wins=0, losses=0;
  for (nsteps_t resultStep = 0;; resultStep++) {
    printf("%d\t%" PRId64 "\t%" PRId64 "\n", resultStep, winsPerResultStep[resultStep], lossesPerResultStep[resultStep]);
    wins += winsPerResultStep[resultStep];
    losses += lossesPerResultStep[resultStep];
    if (winsPerResultStep[resultStep] == 0 && lossesPerResultStep[resultStep] == 0) {
      printf("Total\t%" PRId64 "\t%" PRId64 "\n", wins, losses);
      break;
    }
  }
}
