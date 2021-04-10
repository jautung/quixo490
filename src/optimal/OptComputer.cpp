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

#define MAX_THREADS (16) // maximum that we will support for now
#define BLOCK_TASK_FACTOR (10) // subdivide work into BLOCK_TASK_FACTOR * numThreads tasks, then leave it to OpenMP to allocate from there (too small means large task overhead; too large means load imbalance)

namespace {
  std::chrono::high_resolution_clock::duration initClassTime = std::chrono::high_resolution_clock::duration::zero();
  std::chrono::high_resolution_clock::duration checkTerminalsClassTime = std::chrono::high_resolution_clock::duration::zero();
  std::chrono::high_resolution_clock::duration parentLinkCacheClassTime = std::chrono::high_resolution_clock::duration::zero();
  std::chrono::high_resolution_clock::duration valueIterateClassTime = std::chrono::high_resolution_clock::duration::zero();
  std::chrono::high_resolution_clock::duration elimWinOrDrawClassTime = std::chrono::high_resolution_clock::duration::zero();

  std::chrono::high_resolution_clock::duration initClassPerThreadTaskTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration checkTerminalsClassPerThreadTaskTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration parentLinkCacheClassPerThreadTaskTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration valueIterateClassPerThreadTaskTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration elimWinOrDrawClassPerThreadTaskTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };

  int initClassPerThreadNumTasks[MAX_THREADS] = { 0 };
  int checkTerminalsClassPerThreadNumTasks[MAX_THREADS] = { 0 };
  int parentLinkCacheClassPerThreadNumTasks[MAX_THREADS] = { 0 };
  int valueIterateClassPerThreadNumTasks[MAX_THREADS] = { 0 };
  int elimWinOrDrawClassPerThreadNumTasks[MAX_THREADS] = { 0 };

  std::chrono::high_resolution_clock::duration checkTerminalsClassPerThreadWaitLockTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration parentLinkCacheClassPerThreadWaitLockTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration valueIterateClassPerThreadWaitLockTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };

  std::chrono::high_resolution_clock::duration checkTerminalsClassPerThreadInCritSecTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration parentLinkCacheClassPerThreadInCritSecTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration valueIterateClassPerThreadInCritSecTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };

  std::chrono::high_resolution_clock::duration numStatesClassPerThreadTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration indexToStatePerThreadTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration stateToIndex0PerThreadTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration stateToIndex1PerThreadTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration stateToIndex2PerThreadTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration unfilterOStatePerThreadTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
  std::chrono::high_resolution_clock::duration filterOStatePerThreadTimes[MAX_THREADS] = { std::chrono::high_resolution_clock::duration::zero() };
}

#if OPT_COMPUTE_SPEED_CHECKING == 1
  #define START_TIMING(startTimeVar) {startTimeVar = std::chrono::high_resolution_clock::now();}
  #define END_TIMING(totalTimeVar, startTimeVar) {totalTimeVar += std::chrono::high_resolution_clock::now() - startTimeVar;}
  #define END_TIMING_TASK(perThreadTaskTimesVar, perThreadNumTasksVar, startTimeVar) { \
    perThreadTaskTimesVar[omp_get_thread_num()] += std::chrono::high_resolution_clock::now() - startTimeVar; \
    perThreadNumTasksVar[omp_get_thread_num()] += 1; \
  }
#else
  #define START_TIMING(startTimeVar) { }
  #define END_TIMING(totalTimeVar, startTimeVar) { }
  #define END_TIMING_TASK(perThreadTaskTimesVar, perThreadNumTasksVar, startTimeVar) { }
#endif

OptComputer::OptComputer(GameStateHandler* initGameStateHandler, int initNumThreads, int initNumLocksPerArr) {
  gameStateHandler = initGameStateHandler;
  numTiles = (gameStateHandler->len) * (gameStateHandler->len);
  numThreads = initNumThreads;
  numLocksPerArr = initNumLocksPerArr;

  ncrCalculator = new NcrCalculator(numTiles);
  ordCalculator = new OrdCalculator(numTiles);
  dataHandler = new DataHandler();

  omp_set_num_threads(numThreads);

  #if OPT_COMPUTE_MEMORY_CHECKING == 1
    memoryChecker = new MemoryChecker();
  #endif
}

OptComputer::~OptComputer() {
  delete ncrCalculator;
  delete ordCalculator;
  delete dataHandler;

  #if OPT_COMPUTE_MEMORY_CHECKING == 1
    delete memoryChecker;
  #endif
}

void OptComputer::computeAll(nbit_t numUsedComputeTill) {
  #if OPT_COMPUTE_MEMORY_CHECKING == 1
    memoryChecker->checkVmRss("Initial");
  #endif

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

      #if OPT_COMPUTE_MEMORY_CHECKING == 1
        memoryChecker->checkVmRss("Before loading for class (" + std::to_string(numA) + ", " + std::to_string(numB) + ")");
      #endif

      if (numUsed != numTiles) {
        resultsCacheNormPlus = dataHandler->loadClass(gameStateHandler->len, numB, numA+1); // (numA, numB) -- +1 --> (numB, numA+1)

        #if OPT_COMPUTE_MEMORY_CHECKING == 1
          auto numCacheStatesLoaded = numStatesClass(numA+1, numB);
          memoryChecker->checkVector(&(*resultsCacheNormPlus.begin()), &(*resultsCacheNormPlus.end()), "resultsCacheNormPlus (" + std::to_string(numCacheStatesLoaded) + " states)");
          numCacheStatesLoadedTotal += numCacheStatesLoaded;
        #endif

        if (numA != numB) {
          resultsCacheFlipPlus = dataHandler->loadClass(gameStateHandler->len, numA, numB+1); // (numB, numA) -- +1 --> (numA, numB+1)
          #if OPT_COMPUTE_MEMORY_CHECKING == 1
            auto numCacheStatesLoaded = numStatesClass(numA, numB+1);
            memoryChecker->checkVector(&(*resultsCacheFlipPlus.begin()), &(*resultsCacheFlipPlus.end()), "resultsCacheFlipPlus (" + std::to_string(numCacheStatesLoaded) + " states)");
            numCacheStatesLoadedTotal += numCacheStatesLoaded;
          #endif
        }
      }

      #if OPT_COMPUTE_MEMORY_CHECKING == 1
        memoryChecker->checkVmRss("Loaded " + std::to_string(numCacheStatesLoadedTotal) + " cached states for class (" + std::to_string(numA) + ", " + std::to_string(numB) + ")");
      #endif

      computeClass(numA, numB, resultsCacheNormPlus, resultsCacheFlipPlus);
    }

    if (numUsed == numUsedComputeTill) {
      std::cout << "\n";
      break;
    }
  }

  #if OPT_COMPUTE_SPEED_CHECKING == 1
    std::cout << "Total initClass() time (s)            : " << std::chrono::duration_cast<std::chrono::milliseconds>(initClassTime).count()/1000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << std::chrono::duration_cast<std::chrono::milliseconds>(initClassPerThreadTaskTimes[i]).count()/1000.0 << " (" << initClassPerThreadNumTasks[i] << " tasks)" << "\n";
    }
    std::cout << "Total checkTerminalsClass() time (s)  : " << std::chrono::duration_cast<std::chrono::milliseconds>(checkTerminalsClassTime).count()/1000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << std::chrono::duration_cast<std::chrono::milliseconds>(checkTerminalsClassPerThreadTaskTimes[i]).count()/1000.0 << " (" << checkTerminalsClassPerThreadNumTasks[i] << " tasks)" << "\n";
      std::cout << "   ↳ Total time waiting for locks (s) : " << std::chrono::duration_cast<std::chrono::milliseconds>(checkTerminalsClassPerThreadWaitLockTimes[i]).count()/1000.0 << "\n";
      std::cout << "   ↳ Total time in CSes (s)           : " << std::chrono::duration_cast<std::chrono::milliseconds>(checkTerminalsClassPerThreadInCritSecTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << "Total parentLinkCacheClass() time (s) : " << std::chrono::duration_cast<std::chrono::milliseconds>(parentLinkCacheClassTime).count()/1000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << std::chrono::duration_cast<std::chrono::milliseconds>(parentLinkCacheClassPerThreadTaskTimes[i]).count()/1000.0 << " (" << parentLinkCacheClassPerThreadNumTasks[i] << " tasks)" << "\n";
      std::cout << "   ↳ Total time waiting for locks (s) : " << std::chrono::duration_cast<std::chrono::milliseconds>(parentLinkCacheClassPerThreadWaitLockTimes[i]).count()/1000.0 << "\n";
      std::cout << "   ↳ Total time in CSes (s)           : " << std::chrono::duration_cast<std::chrono::milliseconds>(parentLinkCacheClassPerThreadInCritSecTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << "Total valueIterateClass() time (s)    : " << std::chrono::duration_cast<std::chrono::milliseconds>(valueIterateClassTime).count()/1000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << std::chrono::duration_cast<std::chrono::milliseconds>(valueIterateClassPerThreadTaskTimes[i]).count()/1000.0 << " (" << valueIterateClassPerThreadNumTasks[i] << " tasks)" << "\n";
      std::cout << "   ↳ Total time waiting for locks (s) : " << std::chrono::duration_cast<std::chrono::milliseconds>(valueIterateClassPerThreadWaitLockTimes[i]).count()/1000.0 << "\n";
      std::cout << "   ↳ Total time in CSes (s)           : " << std::chrono::duration_cast<std::chrono::milliseconds>(valueIterateClassPerThreadInCritSecTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << "Total elimWinOrDrawClass() time (s)   : " << std::chrono::duration_cast<std::chrono::milliseconds>(elimWinOrDrawClassTime).count()/1000.0 << "\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << " ↳ Thread " << i << " total task time (s)       : " << std::chrono::duration_cast<std::chrono::milliseconds>(elimWinOrDrawClassPerThreadTaskTimes[i]).count()/1000.0 << " (" << elimWinOrDrawClassPerThreadNumTasks[i] << " tasks)" << "\n";
    }
    std::cout << "\n";
    std::cout << "Auxiliary functions:\n";
    std::cout << " ↳ numStatesClass():\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << "   ↳ Thread " << i << " total time (s)          : " << std::chrono::duration_cast<std::chrono::milliseconds>(numStatesClassPerThreadTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << " ↳ indexToState():\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << "   ↳ Thread " << i << " total time (s)          : " << std::chrono::duration_cast<std::chrono::milliseconds>(indexToStatePerThreadTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << " ↳ stateToIndex0():\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << "   ↳ Thread " << i << " total time (s)          : " << std::chrono::duration_cast<std::chrono::milliseconds>(stateToIndex1PerThreadTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << " ↳ stateToIndex1():\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << "   ↳ Thread " << i << " total time (s)          : " << std::chrono::duration_cast<std::chrono::milliseconds>(stateToIndex1PerThreadTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << " ↳ stateToIndex2():\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << "   ↳ Thread " << i << " total time (s)          : " << std::chrono::duration_cast<std::chrono::milliseconds>(stateToIndex2PerThreadTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << " ↳ unfilterOState():\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << "   ↳ Thread " << i << " total time (s)          : " << std::chrono::duration_cast<std::chrono::milliseconds>(unfilterOStatePerThreadTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << " ↳ filterOState():\n";
    for (int i = 0; i < numThreads; i++) {
      std::cout << "   ↳ Thread " << i << " total time (s)          : " << std::chrono::duration_cast<std::chrono::milliseconds>(filterOStatePerThreadTimes[i]).count()/1000.0 << "\n";
    }
    std::cout << "\n";
  #endif
}

sindex_t OptComputer::numStatesClass(nbit_t numA, nbit_t numB) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf;
  START_TIMING(startTimeBuf);
  sindex_t numStates = (sindex_t)ncrCalculator->ncr(numTiles, numA) * (sindex_t)ncrCalculator->ncr(numTiles-numA, numB);
  END_TIMING(numStatesClassPerThreadTimes[omp_get_thread_num()], startTimeBuf);
  return numStates;
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
  omp_lock_t resultsNormLocks[numLocksPerArr];
  omp_lock_t resultsFlipLocks[numLocksPerArr];
  for (int i = 0; i < numLocksPerArr; i++) omp_init_lock(&resultsNormLocks[i]);
  for (int i = 0; i < numLocksPerArr; i++) omp_init_lock(&resultsFlipLocks[i]);

  #pragma omp parallel
  {
    #pragma omp single
    {
      std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf;

      START_TIMING(startTimeBuf);
      initClass(numA, numB, resultsNorm);
      if (numA != numB) {
        initClass(numB, numA, resultsFlip);
      }
      #pragma omp taskwait
      END_TIMING(initClassTime, startTimeBuf);

      START_TIMING(startTimeBuf);
      if (numA != numB) {
        checkTerminalsClass(numA, numB, resultsNorm, resultsFlip, resultsNormLocks, resultsFlipLocks);
        checkTerminalsClass(numB, numA, resultsFlip, resultsNorm, resultsFlipLocks, resultsNormLocks);
      } else {
        checkTerminalsClass(numA, numB, resultsNorm, resultsNorm, resultsNormLocks, resultsNormLocks);
      }
      #pragma omp taskwait
      END_TIMING(checkTerminalsClassTime, startTimeBuf);

      START_TIMING(startTimeBuf);
      parentLinkCacheClass(numA, numB, resultsNorm, resultsCacheNormPlus, resultsNormLocks); // parent link optimization
      if (numA != numB) {
        parentLinkCacheClass(numB, numA, resultsFlip, resultsCacheFlipPlus, resultsFlipLocks);
      }
      #pragma omp taskwait
      END_TIMING(parentLinkCacheClassTime, startTimeBuf);

      START_TIMING(startTimeBuf);
      while (true) {
        bool updateMade = false;
        if (numA != numB) {
          valueIterateClass(numA, numB, resultsNorm, resultsFlip, resultsCacheNormPlus, resultsNormLocks, resultsFlipLocks, updateMade); // resultsReference of resultsNorm is resultsFlip for MKIND_ZERO and resultsCacheNormPlus for MKIND_PLUS
          valueIterateClass(numB, numA, resultsFlip, resultsNorm, resultsCacheFlipPlus, resultsFlipLocks, resultsNormLocks, updateMade); // resultsReference of resultsFlip is resultsNorm for MKIND_ZERO and resultsCacheFlipPlus for MKIND_PLUS
        } else {
          valueIterateClass(numA, numB, resultsNorm, resultsNorm, resultsCacheNormPlus, resultsNormLocks, resultsNormLocks, updateMade); // resultsReference of resultsNorm is resultsNorm for MKIND_ZERO and resultsCacheNormPlus for MKIND_PLUS
        }
        #pragma omp taskwait
        if (!updateMade) {
          break;
        }
      }
      END_TIMING(valueIterateClassTime, startTimeBuf);

      START_TIMING(startTimeBuf);
      elimWinOrDrawClass(numA, numB, resultsNorm);
      if (numA != numB) {
        elimWinOrDrawClass(numB, numA, resultsFlip);
      }
      #pragma omp taskwait
      END_TIMING(elimWinOrDrawClassTime, startTimeBuf);
    }
  }

  for (int i = 0; i < numLocksPerArr; i++) omp_destroy_lock(&resultsNormLocks[i]);
  for (int i = 0; i < numLocksPerArr; i++) omp_destroy_lock(&resultsFlipLocks[i]);

  auto endTime = std::chrono::high_resolution_clock::now();
  std::cout << "Class (" << +numA << ", " << +numB << ") compute time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count()/1000.0 << "\n";

  dataHandler->saveClass(resultsNorm, gameStateHandler->len, numA, numB);
  if (numA != numB) {
    dataHandler->saveClass(resultsFlip, gameStateHandler->len, numB, numA);
  }
}

void OptComputer::initClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results) {
  #pragma omp task shared(results, initClassPerThreadTaskTimes, initClassPerThreadNumTasks) firstprivate(numX, numO)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf0;
    START_TIMING(startTimeBuf0);
    sindex_t numStates = numStatesClass(numX, numO);
    for (sindex_t stateIndex = 0; stateIndex < numStates/4 + (numStates%4 != 0); stateIndex++) {
      results.push_back(RESULT_DRAW | RESULT_DRAW << 2 | RESULT_DRAW << 4 | RESULT_DRAW << 6); // no lock because only one thread working on results
    }
    END_TIMING_TASK(initClassPerThreadTaskTimes, initClassPerThreadNumTasks, startTimeBuf0);
  }
}

void OptComputer::checkTerminalsClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther, omp_lock_t resultsLocks[], omp_lock_t resultsOtherLocks[]) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf1;

  sindex_t numStates = numStatesClass(numX, numO);
  sindex_t blockTaskInterval = BLOCK_TASK_FACTOR * numThreads; // each singular stateIndex is too small a task, so this 'block division' of tasks works faster for parallelization
  for (sindex_t stateIndexBase = 0; stateIndexBase < blockTaskInterval && stateIndexBase < numStates; stateIndexBase += 1) {
    #pragma omp task shared(results, resultsOther, checkTerminalsClassPerThreadTaskTimes, checkTerminalsClassPerThreadNumTasks, checkTerminalsClassPerThreadWaitLockTimes, checkTerminalsClassPerThreadInCritSecTimes) firstprivate(numX, numO, stateIndexBase) private(startTimeBuf1)
    {
      std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf0;
      START_TIMING(startTimeBuf0);

      for (sindex_t stateIndex = stateIndexBase; stateIndex < numStates; stateIndex += blockTaskInterval) {
        auto state = indexToState(stateIndex, numX, numO);

        if (gameStateHandler->containsLine(state, TILE_X)) {
          START_TIMING(startTimeBuf1);
          omp_set_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
          END_TIMING(checkTerminalsClassPerThreadWaitLockTimes[omp_get_thread_num()], startTimeBuf1);
          START_TIMING(startTimeBuf1);
          dataHandler->setResult(results, stateIndex, RESULT_WIN);
          omp_unset_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
          END_TIMING(checkTerminalsClassPerThreadInCritSecTimes[omp_get_thread_num()], startTimeBuf1);
        }

        else if (gameStateHandler->containsLine(state, TILE_O)) {
          START_TIMING(startTimeBuf1);
          omp_set_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
          END_TIMING(checkTerminalsClassPerThreadWaitLockTimes[omp_get_thread_num()], startTimeBuf1);
          START_TIMING(startTimeBuf1);
          dataHandler->setResult(results, stateIndex, RESULT_LOSS);
          omp_unset_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
          END_TIMING(checkTerminalsClassPerThreadInCritSecTimes[omp_get_thread_num()], startTimeBuf1);
          for (auto parentState : gameStateHandler->allZeroParents(state)) { // parent link optimization
            auto parentStateIndex = stateToIndex(parentState);
            START_TIMING(startTimeBuf1);
            omp_set_lock(&resultsOtherLocks[(parentStateIndex/4) % numLocksPerArr]);
            END_TIMING(checkTerminalsClassPerThreadWaitLockTimes[omp_get_thread_num()], startTimeBuf1);
            START_TIMING(startTimeBuf1);
            if (dataHandler->getResult(resultsOther, parentStateIndex) == RESULT_DRAW) {
              dataHandler->setResult(resultsOther, parentStateIndex, RESULT_WIN);
            }
            omp_unset_lock(&resultsOtherLocks[(parentStateIndex/4) % numLocksPerArr]);
            END_TIMING(checkTerminalsClassPerThreadInCritSecTimes[omp_get_thread_num()], startTimeBuf1);
          }
        }
      }

      END_TIMING_TASK(checkTerminalsClassPerThreadTaskTimes, checkTerminalsClassPerThreadNumTasks, startTimeBuf0);
    }
  }
}

void OptComputer::parentLinkCacheClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsCachePlus, omp_lock_t resultsLocks[]) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf1;

  sindex_t numChildStates = numStatesClass(numO, numX+1);
  sindex_t blockTaskInterval = BLOCK_TASK_FACTOR * numThreads; // each singular childStateIndex is too small a task, so this 'block division' of tasks works faster for parallelization
  for (sindex_t childStateIndexBase = 0; childStateIndexBase < blockTaskInterval && childStateIndexBase < numChildStates; childStateIndexBase++) {
    #pragma omp task shared(results, resultsCachePlus, parentLinkCacheClassPerThreadTaskTimes, parentLinkCacheClassPerThreadNumTasks, parentLinkCacheClassPerThreadWaitLockTimes, parentLinkCacheClassPerThreadInCritSecTimes) firstprivate(numX, numO, childStateIndexBase) private(startTimeBuf1)
    {
      std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf0;
      START_TIMING(startTimeBuf0);

      for (sindex_t childStateIndex = childStateIndexBase; childStateIndex < numChildStates; childStateIndex += blockTaskInterval) {
        auto childResult = dataHandler->getResult(resultsCachePlus, childStateIndex);
        if (childResult == RESULT_LOSS) {
          auto childState = indexToState(childStateIndex, numO, numX+1);
          for (auto state : gameStateHandler->allPlusParents(childState)) {
            auto stateIndex = stateToIndex(state);
            START_TIMING(startTimeBuf1);
            omp_set_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
            END_TIMING(parentLinkCacheClassPerThreadWaitLockTimes[omp_get_thread_num()], startTimeBuf1);
            START_TIMING(startTimeBuf1);
            auto result = dataHandler->getResult(results, stateIndex);
            if (result == RESULT_DRAW || result == RESULT_WIN_OR_DRAW) {
              dataHandler->setResult(results, stateIndex, RESULT_WIN);
            }
            omp_unset_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
            END_TIMING(parentLinkCacheClassPerThreadInCritSecTimes[omp_get_thread_num()], startTimeBuf1);
          }
        } else if (childResult == RESULT_DRAW) {
          auto childState = indexToState(childStateIndex, numO, numX+1);
          for (auto state : gameStateHandler->allPlusParents(childState)) {
            auto stateIndex = stateToIndex(state);
            START_TIMING(startTimeBuf1);
            omp_set_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
            END_TIMING(parentLinkCacheClassPerThreadWaitLockTimes[omp_get_thread_num()], startTimeBuf1);
            START_TIMING(startTimeBuf1);
            if (dataHandler->getResult(results, stateIndex) == RESULT_DRAW) {
              dataHandler->setResult(results, stateIndex, RESULT_WIN_OR_DRAW);
            }
            omp_unset_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
            END_TIMING(parentLinkCacheClassPerThreadInCritSecTimes[omp_get_thread_num()], startTimeBuf1);
          }
        }
      }

      END_TIMING_TASK(parentLinkCacheClassPerThreadTaskTimes, parentLinkCacheClassPerThreadNumTasks, startTimeBuf0);
    }
  }
}

void OptComputer::valueIterateClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther, std::vector<result4_t> &resultsCachePlus, omp_lock_t resultsLocks[], omp_lock_t resultsOtherLocks[], bool &updateMade) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf1;

  sindex_t numStates = numStatesClass(numX, numO);
  sindex_t blockTaskInterval = BLOCK_TASK_FACTOR * numThreads; // each singular stateIndex is too small a task, so this 'block division' of tasks works faster for parallelization
  for (sindex_t stateIndexBase = 0; stateIndexBase < blockTaskInterval && stateIndexBase < numStates; stateIndexBase += 1) {
    #pragma omp task shared(results, resultsOther, resultsCachePlus, updateMade, valueIterateClassPerThreadTaskTimes, valueIterateClassPerThreadNumTasks, valueIterateClassPerThreadWaitLockTimes, valueIterateClassPerThreadInCritSecTimes) firstprivate(numX, numO, stateIndexBase) private(startTimeBuf1)
    {
      std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf0;
      START_TIMING(startTimeBuf0);

      for (sindex_t stateIndex = stateIndexBase; stateIndex < numStates; stateIndex += blockTaskInterval) {
        START_TIMING(startTimeBuf1);
        omp_set_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
        END_TIMING(valueIterateClassPerThreadWaitLockTimes[omp_get_thread_num()], startTimeBuf1);
        START_TIMING(startTimeBuf1);
        auto result = dataHandler->getResult(results, stateIndex);
        omp_unset_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
        END_TIMING(valueIterateClassPerThreadInCritSecTimes[omp_get_thread_num()], startTimeBuf1);
        if (result != RESULT_DRAW) {
          continue;
        }

        auto state = indexToState(stateIndex, numX, numO);
        auto moves = gameStateHandler->allMoves(state);
        bool allChildrenWin = true;
        for (auto move : moves) {
          auto moveKind = gameStateHandler->moveHandler->getKind(move);
          auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
          auto childStateIndex = stateToIndex(childState);
          result_t childResult = RESULT_DRAW; // dummy
          if (moveKind == MKIND_ZERO) {
            START_TIMING(startTimeBuf1);
            omp_set_lock(&resultsOtherLocks[(childStateIndex/4) % numLocksPerArr]);
            END_TIMING(valueIterateClassPerThreadWaitLockTimes[omp_get_thread_num()], startTimeBuf1);
            START_TIMING(startTimeBuf1);
            childResult = dataHandler->getResult(resultsOther, childStateIndex);
            omp_unset_lock(&resultsOtherLocks[(childStateIndex/4) % numLocksPerArr]);
            END_TIMING(valueIterateClassPerThreadInCritSecTimes[omp_get_thread_num()], startTimeBuf1);
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
          START_TIMING(startTimeBuf1);
          omp_set_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
          END_TIMING(valueIterateClassPerThreadWaitLockTimes[omp_get_thread_num()], startTimeBuf1);
          START_TIMING(startTimeBuf1);
          dataHandler->setResult(results, stateIndex, RESULT_LOSS);
          omp_unset_lock(&resultsLocks[(stateIndex/4) % numLocksPerArr]);
          END_TIMING(valueIterateClassPerThreadInCritSecTimes[omp_get_thread_num()], startTimeBuf1);
          for (auto parentState : gameStateHandler->allZeroParents(state)) { // parent link optimization
            auto parentStateIndex = stateToIndex(parentState);
            START_TIMING(startTimeBuf1);
            omp_set_lock(&resultsOtherLocks[(parentStateIndex/4) % numLocksPerArr]);
            END_TIMING(valueIterateClassPerThreadWaitLockTimes[omp_get_thread_num()], startTimeBuf1);
            START_TIMING(startTimeBuf1);
            auto result = dataHandler->getResult(resultsOther, parentStateIndex);
            if (result == RESULT_DRAW || result == RESULT_WIN_OR_DRAW) {
              dataHandler->setResult(resultsOther, parentStateIndex, RESULT_WIN);
            }
            omp_unset_lock(&resultsOtherLocks[(parentStateIndex/4) % numLocksPerArr]);
            END_TIMING(valueIterateClassPerThreadInCritSecTimes[omp_get_thread_num()], startTimeBuf1);
          }
        }
      }

      END_TIMING_TASK(valueIterateClassPerThreadTaskTimes, valueIterateClassPerThreadNumTasks, startTimeBuf0);
    }
  }
}

void OptComputer::elimWinOrDrawClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results) {
  #pragma omp task shared(results, elimWinOrDrawClassPerThreadTaskTimes, elimWinOrDrawClassPerThreadNumTasks) firstprivate(numX, numO)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf0;
    START_TIMING(startTimeBuf0);

    sindex_t numStates = numStatesClass(numX, numO);
    for (sindex_t stateIndex = 0; stateIndex < numStates; stateIndex++) {
      if (dataHandler->getResult(results, stateIndex) == RESULT_WIN_OR_DRAW) { // no lock because only one thread working on results
        dataHandler->setResult(results, stateIndex, RESULT_DRAW);
      }
    }

    END_TIMING_TASK(elimWinOrDrawClassPerThreadTaskTimes, elimWinOrDrawClassPerThreadNumTasks, startTimeBuf0);
  }
}

state_t OptComputer::indexToState(sindex_t stateIndex, nbit_t numX, nbit_t numO) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf;
  START_TIMING(startTimeBuf);

  sindex_t coeff = ncrCalculator->ncr(numTiles-numX, numO);
  ord_t xOrd = stateIndex / coeff;
  ord_t oFilteredOrd = stateIndex % coeff;
  auto xState = ordCalculator->ordPopToState(xOrd, numX);
  auto oFilteredState = ordCalculator->ordPopToState(oFilteredOrd, numO);
  auto oState = unfilterOState(oFilteredState, xState);
  auto state = xState << halfStateNBits | oState;

  END_TIMING(indexToStatePerThreadTimes[omp_get_thread_num()], startTimeBuf);
  return state;
}

sindex_t OptComputer::stateToIndex(state_t state) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf;

  START_TIMING(startTimeBuf);
  auto xState = (state >> halfStateNBits) & halfStateMask;
  auto oState = state & halfStateMask;
  auto oFilteredState = filterOState(oState, xState);
  END_TIMING(stateToIndex0PerThreadTimes[omp_get_thread_num()], startTimeBuf);

  START_TIMING(startTimeBuf);
  auto xOrd = ordCalculator->stateToOrd(xState);
  auto oFilteredOrd = ordCalculator->stateToOrd(oFilteredState);
  END_TIMING(stateToIndex1PerThreadTimes[omp_get_thread_num()], startTimeBuf);

  START_TIMING(startTimeBuf);
  auto numX = gameStateHandler->getNumX(state);
  auto numO = gameStateHandler->getNumO(state);
  sindex_t coeff = ncrCalculator->ncr(numTiles-numX, numO);
  sindex_t stateIndex = xOrd * coeff + oFilteredOrd;
  END_TIMING(stateToIndex2PerThreadTimes[omp_get_thread_num()], startTimeBuf);

  return stateIndex;
}

state_t OptComputer::unfilterOState(state_t oFilteredState, state_t xState) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf;
  START_TIMING(startTimeBuf);

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

  END_TIMING(unfilterOStatePerThreadTimes[omp_get_thread_num()], startTimeBuf);
  return oState;
}

state_t OptComputer::filterOState(state_t oState, state_t xState) {
  std::chrono::time_point<std::chrono::high_resolution_clock> startTimeBuf;
  START_TIMING(startTimeBuf);

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

  END_TIMING(filterOStatePerThreadTimes[omp_get_thread_num()], startTimeBuf);
  return oFilteredState;
}
