#pragma once
#include <unordered_map>

struct nrHash {
  std::size_t operator()(const std::tuple<int, int>& nr) const {
    return std::get<0>(nr) << 5 | std::get<1>(nr);
  }
};

struct nrEqual {
  bool operator()(const std::tuple<int, int>& nr1, const std::tuple<int, int>& nr2) const {
    return std::get<0>(nr1) == std::get<0>(nr2) && std::get<1>(nr1) == std::get<1>(nr2);
  }
};

class NcrCalculator {
  public:
    NcrCalculator(int initLimit);
    int ncr(int n, int r);
  private:
    int limit;
    std::unordered_map<std::tuple<int, int>, int, nrHash, nrEqual> cache;
};
