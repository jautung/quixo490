#pragma once
#include "../game/GameStateHandler.hpp"
#include "NcrCalculator.hpp"
#include "OrdCalculator.hpp"

typedef uint64_t sindex_t; // index of a state

enum result_t : uint8_t {
  RESULT_WIN,
  RESULT_LOSS,
  RESULT_DRAW
};

class OptComputer {
  public:
    OptComputer(nbit_t initNumTiles, GameStateHandler* initGameStateHandler);
    ~OptComputer();
    void computeAll();
  private:
    nbit_t numTiles;
    GameStateHandler* gameStateHandler;
    NcrCalculator* ncrCalculator;
    OrdCalculator* ordCalculator;
    void computeClass(nbit_t numX, nbit_t numO);
    state_t indexToState(sindex_t stateIndex, nbit_t numX, nbit_t numO, sindex_t numStates);
    sindex_t stateToIndex(state_t state);
    state_t unfilterOState(state_t oFilteredState, state_t xState);
    state_t filterOState(state_t oState, state_t xState);
};
