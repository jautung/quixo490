#pragma once
#include "../game/GameStateHandler.hpp"
#include "../utils/DataHandler.hpp"
#include "../utils/MemoryChecker.hpp"
#include "../utils/NcrCalculator.hpp"
#include "OrdCalculator.hpp"
#include <cstdint>
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
    void computeClass(nbit_t numA, nbit_t numB, std::vector<result_t> resultsCacheNormPlus, std::vector<result_t> resultsCacheFlipPlus);
    void initialScanClass(nbit_t numX, nbit_t numO, std::vector<result_t> &results, sindex_t numStates);
    void valueIterateClass(nbit_t numX, nbit_t numO, std::vector<result_t> &results, sindex_t numStates, std::vector<result_t> &resultsOther, std::vector<result_t> &resultsCachePlus, bool &updateMade);
    state_t indexToState(sindex_t stateIndex, nbit_t numX, nbit_t numO);
    state_t unfilterOState(state_t oFilteredState, state_t xState);
    state_t filterOState(state_t oState, state_t xState);
};
