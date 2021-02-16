#pragma once
#include "../game/GameStateHandler.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>

typedef uint64_t ord_t;    // order of an integer
typedef uint64_t ordpop_t; // from LSB to MSB: 8 bits population count, 56 bits order

class OrdCalculator {
  public:
    OrdCalculator(nbit_t initBitLimit);
    state_t ordPopToState(ord_t ord, nbit_t pop);
    ord_t stateToOrd(state_t state);
  private:
    nbit_t bitLimit;
    ordpop_t ordPopCreate(ord_t ord, nbit_t pop);
    std::unordered_map<ordpop_t, state_t> ordPopToStateCache;
    std::vector<ord_t> stateToOrdCache;
};
