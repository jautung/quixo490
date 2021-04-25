#pragma once
#include "../game/GameStateHandler.hpp"
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

typedef uint64_t sindex_t; // index of a state

typedef char byte_t; // raw bytes to write to and read from file
typedef uint16_t nbyte_t; // number of bytes

enum result_t : uint8_t {
  RESULT_WIN,
  RESULT_LOSS,
  RESULT_DRAW,
  RESULT_WIN_OR_DRAW
};

typedef uint8_t result4_t; // block of 4 result_t's

typedef uint8_t nsteps_t; // number of steps to win/lose

class DataHandler {
  public:
    DataHandler();
    std::chrono::high_resolution_clock::duration ioTime;
    result_t getResult(std::vector<result4_t> &results, sindex_t stateIndex);
    nsteps_t getResultStep(std::vector<nsteps_t> &resultsSteps, sindex_t stateIndex);
    void setResult(std::vector<result4_t> &results, sindex_t stateIndex, result_t result);
    void setResultStep(std::vector<nsteps_t> &resultsSteps, sindex_t stateIndex, nsteps_t resultStep);
    void saveClass(std::vector<result4_t> &results, len_t len, nbit_t numX, nbit_t numO);
    void saveClassSteps(std::vector<nsteps_t> &resultsSteps, len_t len, nbit_t numX, nbit_t numO);
    bool existsClass(len_t len, nbit_t numX, nbit_t numO);
    std::vector<result4_t> loadClass(len_t len, nbit_t numX, nbit_t numO);
    std::vector<nsteps_t> loadClassSteps(len_t len, nbit_t numX, nbit_t numO);
    result_t loadState(len_t len, nbit_t numX, nbit_t numO, sindex_t stateIndex);
  private:
    void saveClassAux(std::vector<uint8_t> &results, len_t len, nbit_t numX, nbit_t numO, bool stepsQ);
    std::vector<uint8_t> loadClassAux(len_t len, nbit_t numX, nbit_t numO, bool stepsQ);
    std::string dataFileName(len_t len, nbit_t numX, nbit_t numO, bool stepsQ);
};
