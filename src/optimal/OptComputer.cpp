#include "../game/GameStateHandler.hpp"
#include "../utils/DataHandler.hpp"
#include "../utils/MemoryChecker.hpp"
#include "../utils/NcrCalculator.hpp"
#include "OptComputer.hpp"
#include "OrdCalculator.hpp"
#include <chrono>
#include <iostream>
#include <omp.h>
#include <string>
#include <vector>

namespace {
  nbit_t stateNBits = 64;
  nbit_t halfStateNBits = stateNBits/2;
  state_t halfStateMask = 0b11111111111111111111111111111111;
}

#define MAX_THREADS (16)

namespace {
  bool speedCheck;

  long long initClassTime = 0;
  long long checkTerminalsClassTime = 0;
  long long parentLinkCacheClassTime = 0;
  long long valueIterateClassTime = 0;
  long long elimWinOrDrawClassTime = 0;

  int numThreads;

  long long initClassPerThreadTaskTimes[MAX_THREADS] = { 0 };
  long long checkTerminalsClassPerThreadTaskTimes[MAX_THREADS] = { 0 };
  long long parentLinkCacheClassPerThreadTaskTimes[MAX_THREADS] = { 0 };
  long long valueIterateClassPerThreadTaskTimes[MAX_THREADS] = { 0 };
  long long elimWinOrDrawClassPerThreadTaskTimes[MAX_THREADS] = { 0 };

  int initClassPerThreadNumTasks[MAX_THREADS] = { 0 };
  int checkTerminalsClassPerThreadNumTasks[MAX_THREADS] = { 0 };
  int parentLinkCacheClassPerThreadNumTasks[MAX_THREADS] = { 0 };
  int valueIterateClassPerThreadNumTasks[MAX_THREADS] = { 0 };
  int elimWinOrDrawClassPerThreadNumTasks[MAX_THREADS] = { 0 };

  long long checkTerminalsClassPerThreadInLockTimes[MAX_THREADS] = { 0 };
  long long parentLinkCacheClassPerThreadInLockTimes[MAX_THREADS] = { 0 };
  long long valueIterateClassPerThreadInLockTimes[MAX_THREADS] = { 0 };
}

#define START_TIMING(startTimeVar) {if (speedCheck) startTimeVar = std::chrono::high_resolution_clock::now();}
#define END_TIMING(totalTimeVar, startTimeVar) {if (speedCheck) totalTimeVar += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-startTimeVar).count();}
#define END_TIMING_TASK(perThreadTaskTimesVar, perThreadNumTasksVar, startTimeVar) { \
  if (speedCheck) { \
    perThreadTaskTimesVar[omp_get_thread_num()] += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-startTimeVar).count(); \
    perThreadNumTasksVar[omp_get_thread_num()] += 1; \
  } \
}

OptComputer::OptComputer(nbit_t initNumTiles, GameStateHandler* initGameStateHandler, MemoryChecker* initMemoryChecker, bool initSpeedCheck) {
  numTiles = initNumTiles;
  gameStateHandler = initGameStateHandler;
  memoryChecker = initMemoryChecker;
  ncrCalculator = new NcrCalculator(numTiles);
  ordCalculator = new OrdCalculator(numTiles);
  dataHandler = new DataHandler();
  speedCheck = initSpeedCheck;
}

OptComputer::~OptComputer() {
  delete ncrCalculator;
  delete ordCalculator;
  delete dataHandler;
}

void OptComputer::computeAll() {
  if (memoryChecker) memoryChecker->checkVmRss("Initial");
  for (nbit_t numUsed = numTiles;; numUsed--) {
    for (nbit_t numA = 0; numA <= numUsed/2; numA++) {
      nbit_t numB = numUsed - numA;
      if (dataHandler->existsClass(gameStateHandler->len, numA, numB) && dataHandler->existsClass(gameStateHandler->len, numB, numA)) {
        std::cout << "Class (" << +numA << ", " << +numB << ") already computed\n";
        continue;
      }

      std::vector<result4_t> resultsCacheNormPlus;
      std::vector<result4_t> resultsCacheFlipPlus;
      sindex_t numCacheStatesLoadedTotal = 0;
      if (memoryChecker) memoryChecker->checkVmRss("Before loading for class (" + std::to_string(numA) + ", " + std::to_string(numB) + ")");
      if (numUsed != numTiles) {
        resultsCacheNormPlus = dataHandler->loadClass(gameStateHandler->len, numB, numA+1); // (numA, numB) -- +1 --> (numB, numA+1)
        if (memoryChecker) {
          auto numCacheStatesLoaded = numStatesClass(numA+1, numB);
          memoryChecker->checkVector(&(*resultsCacheNormPlus.begin()), &(*resultsCacheNormPlus.end()), "resultsCacheNormPlus (" + std::to_string(numCacheStatesLoaded) + " states)");
          numCacheStatesLoadedTotal += numCacheStatesLoaded;
        }
        if (numA != numB) {
          resultsCacheFlipPlus = dataHandler->loadClass(gameStateHandler->len, numA, numB+1); // (numB, numA) -- +1 --> (numA, numB+1)
          if (memoryChecker) {
            auto numCacheStatesLoaded = numStatesClass(numA, numB+1);
            memoryChecker->checkVector(&(*resultsCacheFlipPlus.begin()), &(*resultsCacheFlipPlus.end()), "resultsCacheFlipPlus (" + std::to_string(numCacheStatesLoaded) + " states)");
            numCacheStatesLoadedTotal += numCacheStatesLoaded;
          }
        }
      }
      if (memoryChecker) memoryChecker->checkVmRss("Loaded " + std::to_string(numCacheStatesLoadedTotal) + " cached states for class (" + std::to_string(numA) + ", " + std::to_string(numB) + ")");

      computeClass(numA, numB, resultsCacheNormPlus, resultsCacheFlipPlus);
    }
    if (numUsed == 0) {
      break;
    }
  }
  if (speedCheck) {
    std::cout << "Total initClass() time (s)            : " << initClassTime/1000000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << initClassPerThreadTaskTimes[i]/1000000.0 << " (" << initClassPerThreadNumTasks[i] << " tasks)" << "\n";
    }
    std::cout << "Total checkTerminalsClass() time (s)  : " << checkTerminalsClassTime/1000000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << checkTerminalsClassPerThreadTaskTimes[i]/1000000.0 << " (" << checkTerminalsClassPerThreadNumTasks[i] << " tasks)" << "\n";
      std::cout << "   ↳ Total time in locks (s)          : " << checkTerminalsClassPerThreadInLockTimes[i]/1000000.0 << "\n";
    }
    std::cout << "Total parentLinkCacheClass() time (s) : " << parentLinkCacheClassTime/1000000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << parentLinkCacheClassPerThreadTaskTimes[i]/1000000.0 << " (" << parentLinkCacheClassPerThreadNumTasks[i] << " tasks)" << "\n";
      std::cout << "   ↳ Total time in locks (s)          : " << parentLinkCacheClassPerThreadInLockTimes[i]/1000000.0 << "\n";
    }
    std::cout << "Total valueIterateClass() time (s)    : " << valueIterateClassTime/1000000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << valueIterateClassPerThreadTaskTimes[i]/1000000.0 << " (" << valueIterateClassPerThreadNumTasks[i] << " tasks)" << "\n";
      std::cout << "   ↳ Total time in locks (s)          : " << valueIterateClassPerThreadInLockTimes[i]/1000000.0 << "\n";
    }
    std::cout << "Total elimWinOrDrawClass() time (s)   : " << elimWinOrDrawClassTime/1000000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << elimWinOrDrawClassPerThreadTaskTimes[i]/1000000.0 << " (" << elimWinOrDrawClassPerThreadNumTasks[i] << " tasks)" << "\n";
    }
    std::cout << "\n";
  }
}

sindex_t OptComputer::numStatesClass(nbit_t numA, nbit_t numB) {
  return (sindex_t)ncrCalculator->ncr(numTiles, numA) * (sindex_t)ncrCalculator->ncr(numTiles-numA, numB);
}

void OptComputer::computeClass(nbit_t numA, nbit_t numB, std::vector<result4_t> &resultsCacheNormPlus, std::vector<result4_t> &resultsCacheFlipPlus) { // value iteration
  auto startTime = std::chrono::high_resolution_clock::now();

  sindex_t numStates = numStatesClass(numA, numB);
  std::vector<result4_t> resultsNorm;
  std::vector<result4_t> resultsFlip;
  resultsNorm.reserve(numStates/4);
  if (numA != numB) {
    resultsFlip.reserve(numStates/4);
  }
  omp_lock_t resultsNormLock;
  omp_lock_t resultsFlipLock;
  omp_init_lock(&resultsNormLock);
  omp_init_lock(&resultsFlipLock);

  #pragma omp parallel
  {
    #pragma omp single
    {
      numThreads = omp_get_num_threads();
      std::chrono::time_point<std::chrono::high_resolution_clock> startTimeInner;

      START_TIMING(startTimeInner);
      initClass(numA, numB, resultsNorm);
      if (numA != numB) {
        initClass(numB, numA, resultsFlip);
      }
      #pragma omp taskwait
      END_TIMING(initClassTime, startTimeInner);

      START_TIMING(startTimeInner);
      if (numA != numB) {
        checkTerminalsClass(numA, numB, resultsNorm, resultsFlip, resultsNormLock, resultsFlipLock);
        checkTerminalsClass(numB, numA, resultsFlip, resultsNorm, resultsFlipLock, resultsNormLock);
      } else {
        checkTerminalsClass(numA, numB, resultsNorm, resultsNorm, resultsNormLock, resultsNormLock);
      }
      #pragma omp taskwait
      END_TIMING(checkTerminalsClassTime, startTimeInner);

      START_TIMING(startTimeInner);
      parentLinkCacheClass(numA, numB, resultsNorm, resultsCacheNormPlus, resultsNormLock); // parent link optimization
      if (numA != numB) {
        parentLinkCacheClass(numB, numA, resultsFlip, resultsCacheFlipPlus, resultsFlipLock);
      }
      #pragma omp taskwait
      END_TIMING(parentLinkCacheClassTime, startTimeInner);

      START_TIMING(startTimeInner);
      while (true) {
        bool updateMade = false;
        if (numA != numB) {
          valueIterateClass(numA, numB, resultsNorm, resultsFlip, resultsCacheNormPlus, resultsNormLock, resultsFlipLock, updateMade); // resultsReference of resultsNorm is resultsFlip for MKIND_ZERO and resultsCacheNormPlus for MKIND_PLUS
          valueIterateClass(numB, numA, resultsFlip, resultsNorm, resultsCacheFlipPlus, resultsFlipLock, resultsNormLock, updateMade); // resultsReference of resultsFlip is resultsNorm for MKIND_ZERO and resultsCacheFlipPlus for MKIND_PLUS
        } else {
          valueIterateClass(numA, numB, resultsNorm, resultsNorm, resultsCacheNormPlus, resultsNormLock, resultsNormLock, updateMade); // resultsReference of resultsNorm is resultsNorm for MKIND_ZERO and resultsCacheNormPlus for MKIND_PLUS
        }
        #pragma omp taskwait
        if (!updateMade) {
          break;
        }
      }
      END_TIMING(valueIterateClassTime, startTimeInner);

      START_TIMING(startTimeInner);
      elimWinOrDrawClass(numA, numB, resultsNorm);
      if (numA != numB) {
        elimWinOrDrawClass(numB, numA, resultsFlip);
      }
      #pragma omp taskwait
      END_TIMING(elimWinOrDrawClassTime, startTimeInner);
    }
  }

  omp_destroy_lock(&resultsNormLock);
  omp_destroy_lock(&resultsFlipLock);

  auto endTime = std::chrono::high_resolution_clock::now();
  std::cout << "Class (" << +numA << ", " << +numB << ") compute time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count()/1000.0 << "\n";

  dataHandler->saveClass(resultsNorm, gameStateHandler->len, numA, numB);
  if (numA != numB) {
    dataHandler->saveClass(resultsFlip, gameStateHandler->len, numB, numA);
  }
  // std::cout << "Class (" << +numA << ", " << +numB << ") finished saving!\n";
  if (numA == 0 && numB == 0) {
    std::cout << "\n";
  }
}

void OptComputer::initClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results) {
  #pragma omp task shared(results, initClassPerThreadTaskTimes, initClassPerThreadNumTasks) firstprivate(numX, numO)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePerThread;
    START_TIMING(startTimePerThread);
    sindex_t numStates = numStatesClass(numX, numO);
    for (sindex_t stateIndex = 0; stateIndex < numStates/4 + (numStates%4 != 0); stateIndex++) {
      results.push_back(RESULT_DRAW | RESULT_DRAW << 2 | RESULT_DRAW << 4 | RESULT_DRAW << 6); // no lock because only one thread working on results
    }
    END_TIMING_TASK(initClassPerThreadTaskTimes, initClassPerThreadNumTasks, startTimePerThread);
  }
}

void OptComputer::checkTerminalsClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther, omp_lock_t &resultsLock, omp_lock_t &resultsOtherLock) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeInLock;

  sindex_t numStates = numStatesClass(numX, numO);
  for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
    #pragma omp task shared(results, resultsOther, checkTerminalsClassPerThreadTaskTimes, checkTerminalsClassPerThreadNumTasks, checkTerminalsClassPerThreadInLockTimes) firstprivate(numX, numO, stateIndex) private(startTimeInLock)
    {
      std::chrono::time_point<std::chrono::high_resolution_clock> startTimePerThread;
      START_TIMING(startTimePerThread);

      auto state = indexToState(stateIndex, numX, numO);

      if (gameStateHandler->containsLine(state, TILE_X)) {
        START_TIMING(startTimeInLock);
        omp_set_lock(&resultsLock);
        dataHandler->setResult(results, stateIndex, RESULT_WIN);
        omp_unset_lock(&resultsLock);
        END_TIMING(checkTerminalsClassPerThreadInLockTimes[omp_get_thread_num()], startTimeInLock);
      }

      else if (gameStateHandler->containsLine(state, TILE_O)) {
        START_TIMING(startTimeInLock);
        omp_set_lock(&resultsLock);
        dataHandler->setResult(results, stateIndex, RESULT_LOSS);
        omp_unset_lock(&resultsLock);
        END_TIMING(checkTerminalsClassPerThreadInLockTimes[omp_get_thread_num()], startTimeInLock);
        for (auto parentState : gameStateHandler->allZeroParents(state)) { // parent link optimization
          auto parentStateIndex = stateToIndex(parentState);
          START_TIMING(startTimeInLock);
          omp_set_lock(&resultsOtherLock);
          if (dataHandler->getResult(resultsOther, parentStateIndex) == RESULT_DRAW) {
            dataHandler->setResult(resultsOther, parentStateIndex, RESULT_WIN);
          }
          omp_unset_lock(&resultsOtherLock);
          END_TIMING(checkTerminalsClassPerThreadInLockTimes[omp_get_thread_num()], startTimeInLock);
        }
      }

      END_TIMING_TASK(checkTerminalsClassPerThreadTaskTimes, checkTerminalsClassPerThreadNumTasks, startTimePerThread);
    }
  }
}

void OptComputer::parentLinkCacheClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsCachePlus, omp_lock_t &resultsLock) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeInLock;

  sindex_t numChildStates = numStatesClass(numO, numX+1);
  for (sindex_t childStateIndex = 0; childStateIndex < numChildStates; childStateIndex++) {
    auto childResult = dataHandler->getResult(resultsCachePlus, childStateIndex);
    if (childResult == RESULT_LOSS) {
      #pragma omp task shared(results, parentLinkCacheClassPerThreadTaskTimes, parentLinkCacheClassPerThreadNumTasks, parentLinkCacheClassPerThreadInLockTimes) firstprivate(numX, numO, childStateIndex) private(startTimeInLock)
      {
        std::chrono::time_point<std::chrono::high_resolution_clock> startTimePerThread;
        START_TIMING(startTimePerThread);

        auto childState = indexToState(childStateIndex, numO, numX+1);
        for (auto state : gameStateHandler->allPlusParents(childState)) {
          auto stateIndex = stateToIndex(state);
          START_TIMING(startTimeInLock);
          omp_set_lock(&resultsLock);
          auto result = dataHandler->getResult(results, stateIndex);
          if (result == RESULT_DRAW || result == RESULT_WIN_OR_DRAW) {
            dataHandler->setResult(results, stateIndex, RESULT_WIN);
          }
          omp_unset_lock(&resultsLock);
          END_TIMING(parentLinkCacheClassPerThreadInLockTimes[omp_get_thread_num()], startTimeInLock);
        }

        END_TIMING_TASK(parentLinkCacheClassPerThreadTaskTimes, parentLinkCacheClassPerThreadNumTasks, startTimePerThread);
      }
    } else if (childResult == RESULT_DRAW) {
      #pragma omp task shared(results, parentLinkCacheClassPerThreadTaskTimes, parentLinkCacheClassPerThreadNumTasks, parentLinkCacheClassPerThreadInLockTimes) firstprivate(numX, numO, childStateIndex) private(startTimeInLock)
      {
        std::chrono::time_point<std::chrono::high_resolution_clock> startTimePerThread;
        START_TIMING(startTimePerThread);

        auto childState = indexToState(childStateIndex, numO, numX+1);
        for (auto state : gameStateHandler->allPlusParents(childState)) {
          auto stateIndex = stateToIndex(state);
          START_TIMING(startTimeInLock);
          omp_set_lock(&resultsLock);
          if (dataHandler->getResult(results, stateIndex) == RESULT_DRAW) {
            dataHandler->setResult(results, stateIndex, RESULT_WIN_OR_DRAW);
          }
          omp_unset_lock(&resultsLock);
          END_TIMING(parentLinkCacheClassPerThreadInLockTimes[omp_get_thread_num()], startTimeInLock);
        }

        END_TIMING_TASK(parentLinkCacheClassPerThreadTaskTimes, parentLinkCacheClassPerThreadNumTasks, startTimePerThread);
      }
    }
  }
}

void OptComputer::valueIterateClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther, std::vector<result4_t> &resultsCachePlus, omp_lock_t &resultsLock, omp_lock_t &resultsOtherLock, bool &updateMade) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeInLock;

  sindex_t numStates = numStatesClass(numX, numO);
  for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
    if (dataHandler->getResult(results, stateIndex) != RESULT_DRAW) { // no lock because outside any task
      continue;
    }
    #pragma omp task shared(results, resultsOther, resultsCachePlus, updateMade, valueIterateClassPerThreadTaskTimes, valueIterateClassPerThreadNumTasks, valueIterateClassPerThreadInLockTimes) firstprivate(numX, numO, stateIndex) private(startTimeInLock)
    {
      std::chrono::time_point<std::chrono::high_resolution_clock> startTimePerThread;
      START_TIMING(startTimePerThread);

      auto state = indexToState(stateIndex, numX, numO);
      auto moves = gameStateHandler->allMoves(state);
      bool allChildrenWin = true;
      for (auto move : moves) {
        auto moveKind = gameStateHandler->moveHandler->getKind(move);
        auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
        auto childStateIndex = stateToIndex(childState);
        result_t childResult = RESULT_DRAW; // dummy
        if (moveKind == MKIND_ZERO) {
          START_TIMING(startTimeInLock);
          omp_set_lock(&resultsOtherLock);
          childResult = dataHandler->getResult(resultsOther, childStateIndex);
          omp_unset_lock(&resultsOtherLock);
          END_TIMING(valueIterateClassPerThreadInLockTimes[omp_get_thread_num()], startTimeInLock);
        } else if (moveKind == MKIND_PLUS) {
          childResult = dataHandler->getResult(resultsCachePlus, childStateIndex);
        }
        if (childResult != RESULT_WIN) {
          allChildrenWin = false;
          break;
        }
      }
      if (allChildrenWin) {
        updateMade = true;
        START_TIMING(startTimeInLock);
        omp_set_lock(&resultsLock);
        dataHandler->setResult(results, stateIndex, RESULT_LOSS);
        omp_unset_lock(&resultsLock);
        END_TIMING(valueIterateClassPerThreadInLockTimes[omp_get_thread_num()], startTimeInLock);
        for (auto parentState : gameStateHandler->allZeroParents(state)) { // parent link optimization
          auto parentStateIndex = stateToIndex(parentState);
          START_TIMING(startTimeInLock);
          omp_set_lock(&resultsOtherLock);
          auto result = dataHandler->getResult(resultsOther, parentStateIndex);
          if (result == RESULT_DRAW || result == RESULT_WIN_OR_DRAW) {
            dataHandler->setResult(resultsOther, parentStateIndex, RESULT_WIN);
          }
          omp_unset_lock(&resultsOtherLock);
          END_TIMING(valueIterateClassPerThreadInLockTimes[omp_get_thread_num()], startTimeInLock);
        }
      }

      END_TIMING_TASK(valueIterateClassPerThreadTaskTimes, valueIterateClassPerThreadNumTasks, startTimePerThread);
    }
  }
}

void OptComputer::elimWinOrDrawClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results) {
  #pragma omp task shared(results, elimWinOrDrawClassPerThreadTaskTimes, elimWinOrDrawClassPerThreadNumTasks) firstprivate(numX, numO)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePerThread;
    START_TIMING(startTimePerThread);

    sindex_t numStates = numStatesClass(numX, numO);
    for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
      if (dataHandler->getResult(results, stateIndex) == RESULT_WIN_OR_DRAW) { // no lock because only one thread working on results
        dataHandler->setResult(results, stateIndex, RESULT_DRAW);
      }
    }

    END_TIMING_TASK(elimWinOrDrawClassPerThreadTaskTimes, elimWinOrDrawClassPerThreadNumTasks, startTimePerThread);
  }
}

state_t OptComputer::indexToState(sindex_t stateIndex, nbit_t numX, nbit_t numO) {
  sindex_t coeff = ncrCalculator->ncr(numTiles-numX, numO);
  ord_t xOrd = stateIndex / coeff;
  ord_t oFilteredOrd = stateIndex % coeff;
  auto xState = ordCalculator->ordPopToState(xOrd, numX);
  auto oFilteredState = ordCalculator->ordPopToState(oFilteredOrd, numO);
  auto oState = unfilterOState(oFilteredState, xState);
  return xState << halfStateNBits | oState;
}

sindex_t OptComputer::stateToIndex(state_t state) {
  auto xState = (state >> halfStateNBits) & halfStateMask;
  auto oState = state & halfStateMask;
  auto oFilteredState = filterOState(oState, xState);
  auto xOrd = ordCalculator->stateToOrd(xState);
  auto oFilteredOrd = ordCalculator->stateToOrd(oFilteredState);
  auto numX = gameStateHandler->getNumX(state);
  auto numO = gameStateHandler->getNumO(state);
  sindex_t coeff = ncrCalculator->ncr(numTiles-numX, numO);
  return xOrd*coeff + oFilteredOrd;
}

state_t OptComputer::unfilterOState(state_t oFilteredState, state_t xState) {
  state_t mainPointer = 0b1;
  state_t filteredPointer = 0b1;
  state_t oState = 0b0;
  for (nbit_t i = 0; i < numTiles; i++, mainPointer <<= 1) {
    if ((xState & mainPointer) != mainPointer) {
      if ((oFilteredState & filteredPointer) == filteredPointer) {
        oState |= mainPointer;
      }
      filteredPointer <<= 1;
    }
  }
  return oState;
}

state_t OptComputer::filterOState(state_t oState, state_t xState) {
  state_t mainPointer = 0b1;
  state_t filteredPointer = 0b1;
  state_t oFilteredState = 0b0;
  for (nbit_t i = 0; i < numTiles; i++, mainPointer <<= 1) {
    if ((xState & mainPointer) != mainPointer) {
      if ((oState & mainPointer) == mainPointer) {
        oFilteredState |= filteredPointer;
      }
      filteredPointer <<= 1;
    }
  }
  return oFilteredState;
}
