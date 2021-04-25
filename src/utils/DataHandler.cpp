#include "../game/GameStateHandler.hpp"
#include "DataHandler.hpp"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
  nbyte_t byteBufferSize = 16384;
  byte_t resultMask = 0b11;
  std::string dataDirPath = "data/optimal/";
}

DataHandler::DataHandler() {
  ioTime = std::chrono::high_resolution_clock::duration::zero();
}

result_t DataHandler::getResult(std::vector<result4_t> &results, sindex_t stateIndex) {
  return (result_t)((results[stateIndex/4] >> (2 * (stateIndex%4))) & resultMask);
}

nsteps_t DataHandler::getResultStep(std::vector<nsteps_t> &resultsSteps, sindex_t stateIndex) {
  return resultsSteps[stateIndex];
}

void DataHandler::setResult(std::vector<result4_t> &results, sindex_t stateIndex, result_t result) {
  results[stateIndex/4] &= ~(resultMask << (2 * (stateIndex%4)));
  results[stateIndex/4] |= (result << (2 * (stateIndex%4)));
}

void DataHandler::setResultStep(std::vector<nsteps_t> &resultsSteps, sindex_t stateIndex, nsteps_t resultStep) {
  resultsSteps[stateIndex] = resultStep;
}

void DataHandler::saveClass(std::vector<result4_t> &results, len_t len, nbit_t numX, nbit_t numO) {
  saveClassAux(results, len, numX, numO, false);
}

void DataHandler::saveClassSteps(std::vector<nsteps_t> &resultsSteps, len_t len, nbit_t numX, nbit_t numO) {
  saveClassAux(resultsSteps, len, numX, numO, true);
}

void DataHandler::saveClassAux(std::vector<uint8_t> &results, len_t len, nbit_t numX, nbit_t numO, bool stepsQ) {
  auto startTime = std::chrono::high_resolution_clock::now();

  byte_t byteBuffer[byteBufferSize];
  std::ofstream dataFileStream(dataFileName(len, numX, numO, stepsQ), std::ios::out|std::ios::binary);

  nbyte_t byteBufferWriteIndex = 0;
  for (auto result : results) {
    byteBuffer[byteBufferWriteIndex] = result;
    byteBufferWriteIndex += 1;
    if (byteBufferWriteIndex == byteBufferSize) {
      dataFileStream.write(byteBuffer, byteBufferSize);
      byteBufferWriteIndex = 0;
    }
  }
  if (byteBufferWriteIndex != 0) { // remaining bytes in buffer
    dataFileStream.write(byteBuffer, byteBufferWriteIndex);
  }

  dataFileStream.close();

  auto endTime = std::chrono::high_resolution_clock::now();
  ioTime += endTime - startTime;
}

bool DataHandler::existsClass(len_t len, nbit_t numX, nbit_t numO) {
  std::ifstream dataFileStream(dataFileName(len, numX, numO, false), std::ios::in|std::ios::binary);
  bool ret = !dataFileStream.fail();
  dataFileStream.close();
  return ret;
}

std::vector<result4_t> DataHandler::loadClass(len_t len, nbit_t numX, nbit_t numO) {
  return loadClassAux(len, numX, numO, false);
}

std::vector<nsteps_t> DataHandler::loadClassSteps(len_t len, nbit_t numX, nbit_t numO) {
  return loadClassAux(len, numX, numO, true);
}

std::vector<uint8_t> DataHandler::loadClassAux(len_t len, nbit_t numX, nbit_t numO, bool stepsQ) {
  auto startTime = std::chrono::high_resolution_clock::now();

  byte_t byteBuffer[byteBufferSize];
  std::ifstream dataFileStream(dataFileName(len, numX, numO, stepsQ), std::ios::in|std::ios::binary);

  #if OPT_COMPUTE_ERROR_CHECKING == 1
  if (dataFileStream.fail()) {
    std::cerr << "error: " << "data file cannot be opened: " << dataFileName(len, numX, numO, stepsQ) << "\n";
    exit(1);
  }
  #endif

  std::vector<result4_t> results;
  while (dataFileStream) { // reading bytes in byte blocks of byteBufferSize
    dataFileStream.read(byteBuffer, byteBufferSize);
    for (nbyte_t i = 0; i < dataFileStream.gcount(); i++) {
      results.push_back((result_t)(byteBuffer[i]));
    }
  }

  dataFileStream.close();

  auto endTime = std::chrono::high_resolution_clock::now();
  ioTime += endTime - startTime;

  return results;
}

result_t DataHandler::loadState(len_t len, nbit_t numX, nbit_t numO, sindex_t stateIndex) {
  byte_t byteBuffer[1];
  std::ifstream dataFileStream(dataFileName(len, numX, numO, false), std::ios::in|std::ios::binary);

  #if OPT_COMPUTE_ERROR_CHECKING == 1
  if (dataFileStream.fail()) {
    std::cerr << "error: " << "data file cannot be opened: " << dataFileName(len, numX, numO, false) << "\n";
    exit(1);
  }
  #endif

  dataFileStream.seekg(stateIndex/4);
  dataFileStream.read(byteBuffer, 1);
  result_t result = (result_t)((byteBuffer[0] >> (2 * (stateIndex%4))) & resultMask);

  dataFileStream.close();
  return result;
}

std::string DataHandler::dataFileName(len_t len, nbit_t numX, nbit_t numO, bool stepsQ) {
  return dataDirPath + "len" + std::to_string(len) + "_x" + std::to_string(numX) + "_o" + std::to_string(numO) + (stepsQ ? "_steps" : "") + ".dat";
}
