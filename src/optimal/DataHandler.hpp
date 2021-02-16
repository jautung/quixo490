#pragma once
#include "../game/GameStateHandler.hpp"
#include <cstdint>
#include <vector>

typedef uint64_t sindex_t; // index of a state

typedef char byte_t; // raw bytes to write to and read from file
typedef uint16_t nbyte_t; // number of bytes

enum result_t : uint8_t {
  RESULT_WIN,
  RESULT_LOSS,
  RESULT_DRAW
};

class DataHandler {
  public:
    DataHandler();
    int ioTime;
    void saveClass(std::vector<result_t> &results, len_t len, nbit_t numX, nbit_t numO);
    std::vector<result_t> loadClass(len_t len, nbit_t numX, nbit_t numO);
    result_t loadState(len_t len, nbit_t numX, nbit_t numO, sindex_t stateIndex);
  private:
    void loadByte(char byte, std::vector<result_t> &results);
    std::string dataFileName(len_t len, nbit_t numX, nbit_t numO);
};
