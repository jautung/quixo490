#pragma once
#include <iostream>
#include <unordered_map>

struct nrHash {
  std::size_t operator()(const std::tuple<int, int>& nr) const {
    return std::get<0>(nr) << 8 | std::get<1>(nr);
  }
};

struct nrEqual {
  bool operator()(const std::tuple<int, int>& nr1, const std::tuple<int, int>& nr2) const {
    return std::get<0>(nr1) == std::get<0>(nr2) && std::get<1>(nr1) == std::get<1>(nr2);
  }
};

class NcrCalculator {
  public:
    NcrCalculator(int initLimit) {
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
    int ncr(int n, int r) {
      #if OPT_COMPUTE_ERROR_CHECKING == 1
      if (n < 0 || r < 0) {
        std::cerr << "error: ncr: provided n=" << n << " and r=" << r << " are not both non-negative\n";
        exit(1);
      }
      if (n > limit) {
        std::cerr << "error: ncr: provided n=" << n << " is larger than limit=" << limit << "\n";
        exit(1);
      }
      #endif
      if (n < r) {
        return 0;
      }
      return cache[std::make_tuple(n, r)];
    }
  private:
    int limit;
    std::unordered_map<std::tuple<int, int>, int, nrHash, nrEqual> cache;
};
