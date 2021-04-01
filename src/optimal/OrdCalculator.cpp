#include "../game/GameStateHandler.hpp"
#include "OrdCalculator.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

OrdCalculator::OrdCalculator(nbit_t initBitLimit) {
  bitLimit = initBitLimit;

  std::vector<ord_t> countPerPop;
  countPerPop.resize(bitLimit+1, 0); // bitLimit+1 possible number of pop's
  stateToOrdCache.reserve(0b1 << bitLimit);
  for (state_t state = 0; state < (state_t)0b1 << bitLimit; state++) {
    nbit_t pop = __builtin_popcount(state);
    ord_t ord = countPerPop[pop];
    ordPopToStateCache[ordPopCreate(ord, pop)] = state;
    stateToOrdCache.push_back(ord);
    countPerPop[pop] += 1;
  }
}

ordpop_t OrdCalculator::ordPopCreate(ord_t ord, nbit_t pop) {
  return pop | ord << 8;
}

state_t OrdCalculator::ordPopToState(ord_t ord, nbit_t pop) {
  #if OPT_COMPUTE_ERROR_CHECKING == 1
  if (ord < 0 || pop < 0) {
    std::cerr << "error: ord: provided ord=" << ord << " and pop=" << +pop << " are not both non-negative\n";
    exit(1);
  }
  if (pop > bitLimit) {
    std::cerr << "error: ord: provided pop=" << +pop << " is larger than limit=" << +bitLimit << "\n";
    exit(1);
  }
  #endif

  ordpop_t ordPop = ordPopCreate(ord, pop);

  #if OPT_COMPUTE_ERROR_CHECKING == 1
  if (ordPopToStateCache.find(ordPop) == ordPopToStateCache.end()) {
    std::cerr << "error: ord: provided ord=" << ord << " is invalid with pop=" << +pop << "\n";
    exit(1);
  }
  #endif

  return ordPopToStateCache[ordPop];
}

ord_t OrdCalculator::stateToOrd(state_t state) {
  #if OPT_COMPUTE_ERROR_CHECKING == 1
  if (state < 0) {
    std::cerr << "error: ord: provided state=" << state << " is not non-negative\n";
    exit(1);
  }
  if (state >= (state_t)0b1 << bitLimit) {
    std::cerr << "error: ord: provided state=" << state << " is larger than limit=" << (0b1 << bitLimit) << "\n";
    exit(1);
  }
  #endif

  return stateToOrdCache[state];
}
