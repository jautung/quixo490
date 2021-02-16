#include "../game/GameStateHandler.hpp"
#include "OrdCalculator.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

OrdCalculator::OrdCalculator(nbit_t initBitLimit) {
  bitLimit = initBitLimit;

  std::vector<ord_t> countPerPop;
  countPerPop.resize(bitLimit, 0);
  stateToOrdCache.reserve(0b1 << bitLimit);
  for (state_t state = 0; state < 0b1 << bitLimit; state++) {
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
  if (ord < 0 || pop < 0) {
    std::cerr << "error: ord: provided ord=" << ord << " and pop=" << +pop << " are not both non-negative\n";
    return 0;
  }
  if (pop > bitLimit) {
    std::cerr << "error: ord: provided pop=" << +pop << " is larger than limit=" << +bitLimit << "\n";
    return 0;
  }
  ordpop_t ordPop = ordPopCreate(ord, pop);
  if (ordPopToStateCache.find(ordPop) == ordPopToStateCache.end()) {
    std::cerr << "error: ord: provided ord=" << ord << " is invalid with pop=" << +pop << "\n";
    return 0;
  }
  return ordPopToStateCache[ordPop];
}

ord_t OrdCalculator::stateToOrd(state_t state) {
  if (state < 0) {
    std::cerr << "error: ord: provided state=" << state << " is not non-negative\n";
    return 0;
  }
  if (state >= 0b1 << bitLimit) {
    std::cerr << "error: ord: provided state=" << state << " is larger than limit=" << (0b1 << bitLimit) << "\n";
    return 0;
  }
  return stateToOrdCache[state];
}
