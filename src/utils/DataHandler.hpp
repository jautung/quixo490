#pragma once
#include "../game/GameStateHandler.hpp"
#include <cstdint>
#include <string>
#include <vector>

typedef uint64_t sindex_t; // index of a state

typedef char byte_t; // raw bytes to write to and read from file
typedef uint16_t nbyte_t; // number of bytes

enum result_t : uint8_t {
  RESULT_WIN,
  RESULT_LOSS,
  RESULT_DRAW
};

typedef uint8_t result4_t; // block of 4 result_t's

class DataHandler {
  public:
    DataHandler();
    int ioTime;
    result_t getResult(std::vector<result4_t> &results, sindex_t stateIndex);
    void setResult(std::vector<result4_t> &results, sindex_t stateIndex, result_t result);
    void saveClass(std::vector<result4_t> &results, len_t len, nbit_t numX, nbit_t numO);
    bool existsClass(len_t len, nbit_t numX, nbit_t numO);
    std::vector<result4_t> loadClass(len_t len, nbit_t numX, nbit_t numO);
    result_t loadState(len_t len, nbit_t numX, nbit_t numO, sindex_t stateIndex);
  private:
    std::string dataFileName(len_t len, nbit_t numX, nbit_t numO);
};
