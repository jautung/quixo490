#pragma once
#include "../game/GameStateHandler.hpp"
#include <vector>

enum result_t : uint8_t {
  RESULT_WIN,
  RESULT_LOSS,
  RESULT_DRAW
};

class DataHandler {
  public:
    void saveData(std::vector<result_t> results, len_t len, nbit_t numX, nbit_t numO);
    std::vector<result_t> loadData(len_t len, nbit_t numX, nbit_t numO);
};
