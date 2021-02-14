#include "../game/GameState.hpp"
#include "OrdCalculator.hpp"
#include <iostream>
#include <vector>

#define BIT_COUNT_NBITS (5) // TODO: these depend on initBitLimit actually
#define ORD_NBITS (23)

OrdCalculator::OrdCalculator(int initBitLimit) {
  bitLimit = initBitLimit;
  ordToStateCache.reserve(0b1 << (BIT_COUNT_NBITS + ORD_NBITS));
  stateToOrdCache.reserve(0b1 << bitLimit);

  std::vector<int> bitCountSeen;
  bitCountSeen.reserve(bitLimit);
  for (state_t state = 0; state < 0b1 << bitLimit; state++) {
    int bitCount = __builtin_popcount(state);
    int ord = bitCountSeen[bitCount];
    ordToStateCache[ordBitCountIndex(ord, bitCount)] = state;
    stateToOrdCache[state] = ord;
    bitCountSeen[bitCount] += 1;
  }
}

int OrdCalculator::ordBitCountIndex(int ord, int bitCount) {
  return bitCount | ord << BIT_COUNT_NBITS;
}

state_t OrdCalculator::ordToState(int ord, int bitCount) {
  if (ord < 0 || bitCount < 0) {
    std::cerr << "error: ord: provided ord=" << ord << " and bitCount=" << bitCount << " are not both non-negative\n";
    return -1;
  }
  if (bitCount > bitLimit) {
    std::cerr << "error: ord: provided bitCount=" << bitCount << " is larger than limit=" << bitLimit << "\n";
    return -1;
  }
  return ordToStateCache[ordBitCountIndex(ord, bitCount)];
}

int OrdCalculator::stateToOrd(state_t state) {
  if (state < 0) {
    std::cerr << "error: ord: provided state=" << state << "is not non-negative\n";
    return -1;
  }
  if (state >= 0b1 << bitLimit) {
    std::cerr << "error: ord: provided state=" << state << " is larger than limit=" << (0b1 << bitLimit) << "\n";
    return -1;
  }
  return stateToOrdCache[state];
}
