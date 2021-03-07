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
    OptComputer(nbit_t initNumTiles, GameStateHandler* initGameStateHandler, MemoryChecker* initMemoryChecker = NULL);
    ~OptComputer();
    void computeAll();
    sindex_t stateToIndex(state_t state);
    DataHandler* dataHandler;
  private:
    nbit_t numTiles;
    GameStateHandler* gameStateHandler;
    MemoryChecker* memoryChecker;
    NcrCalculator* ncrCalculator;
    OrdCalculator* ordCalculator;
    sindex_t numStatesClass(nbit_t numA, nbit_t numB);
    void computeClass(nbit_t numA, nbit_t numB, std::vector<result4_t> resultsCacheNormPlus, std::vector<result4_t> resultsCacheFlipPlus);
    void initClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results);
    void checkTerminalsClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther, omp_lock_t &resultsLock, omp_lock_t &resultsOtherLock);
    void parentLinkCacheClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsCachePlus, omp_lock_t &resultsLock);
    void valueIterateClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results, std::vector<result4_t> &resultsOther, std::vector<result4_t> &resultsCachePlus, omp_lock_t &resultsLock, omp_lock_t &resultsOtherLock, bool &updateMade);
    void elimWinOrDrawClass(nbit_t numX, nbit_t numO, std::vector<result4_t> &results);
    state_t indexToState(sindex_t stateIndex, nbit_t numX, nbit_t numO);
    state_t unfilterOState(state_t oFilteredState, state_t xState);
    state_t filterOState(state_t oState, state_t xState);
};
