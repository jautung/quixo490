#pragma once
#include "../game/GameStateHandler.hpp"
#include "../utils/DataHandler.hpp"
#include "../utils/MemoryChecker.hpp"
#include "../utils/NcrCalculator.hpp"
#include "OrdCalculator.hpp"
#include <cstdint>
#include <omp.h>
#include <string>
#include <vector>

class OptComputer {
  public:
    OptComputer(GameStateHandler* initGameStateHandler, int initNumThreads = 1, int initNumLocksPerArr = 1, bool initConsiderStepsQ = false);
    ~OptComputer();
    void computeAll(nbit_t numUsedComputeTill = 0);
    sindex_t stateToIndex(state_t state);
    sindex_t numStatesClass(nbit_t numA, nbit_t numB);
    DataHandler* dataHandler;
  private:
    GameStateHandler* gameStateHandler;
    nbit_t numTiles;
    int numThreads;
    int numLocksPerArr;
    bool considerStepsQ;
    NcrCalculator* ncrCalculator;
    OrdCalculator* ordCalculator;
    MemoryChecker* memoryChecker;
    void computeClass(nbit_t numA, nbit_t numB, std::vector<result4_t> &resultsCacheNormPlus, std::vector<result4_t> &resultsCacheFlipPlus, std::vector<nsteps_t> &resultsStepsCacheNormPlus, std::vector<nsteps_t> &resultsStepsCacheFlipPlus);
    void initClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<nsteps_t> &resultsSteps);
    void checkTerminalsClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther, std::vector<nsteps_t> &resultsSteps, std::vector<nsteps_t> &resultsStepsOther, omp_lock_t resultsLocks[], omp_lock_t resultsOtherLocks[]);
    void parentLinkCacheClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<nsteps_t> &resultsSteps, std::vector<result4_t> &resultsCachePlus, std::vector<nsteps_t> &resultsStepsCachePlus, omp_lock_t resultsLocks[]);
    void valueIterateClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther, std::vector<nsteps_t> &resultsSteps, std::vector<nsteps_t> &resultsStepsOther, std::vector<result4_t> &resultsCachePlus, std::vector<nsteps_t> &resultsStepsCachePlus, omp_lock_t resultsLocks[], omp_lock_t resultsOtherLocks[], bool &updateMade);
    void elimWinOrDrawClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results);
    state_t indexToState(sindex_t stateIndex, nbit_t numX, nbit_t numO);
    state_t unfilterOState(state_t oFilteredState, state_t xState);
    state_t filterOState(state_t oState, state_t xState);
    bool resultStepWinUpdate(std::vector<nsteps_t> &resultsSteps, sindex_t stateIndex, nsteps_t lossChildResultStep);
};
