#include "NcrCalculator.hpp"
#include <iostream>
#include <unordered_map>

NcrCalculator::NcrCalculator(int initLimit) {
  limit = initLimit;
  for (int n = 0; n <= limit; n++) {
    for (int r = 0; r <= n; r++) {
      if (n == 0) {
        cache[std::make_tuple(n, r)] = 1;
      } else if (n == r) {
        cache[std::make_tuple(n, r)] = 1;
      } else {
        cache[std::make_tuple(n, r)] = cache[std::make_tuple(n-1, r)] + cache[std::make_tuple(n-1, r-1)];
      }
    }
  }
}

int NcrCalculator::ncr(int n, int r) {
  if (n < 0 || r < 0) {
    std::cerr << "error: ncr: provided n=" << n << " and r=" << r << " are not both non-negative\n";
    return -1;
  }
  if (n > limit) {
    std::cerr << "error: ncr: provided n=" << n << " is larger than limit=" << limit << "\n";
    return -1;
  }
  if (n < r) {
    return 0;
  }
  return cache[std::make_tuple(n, r)];
}
