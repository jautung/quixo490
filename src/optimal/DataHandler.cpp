#include "../game/GameStateHandler.hpp"
#include "DataHandler.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
  nbyte_t byteBufferSize = 1024;
  byte_t resultMask = 0b11;
  std::string dataDirPath = "data/optimal/";
}

void DataHandler::saveClass(std::vector<result_t> &results, len_t len, nbit_t numX, nbit_t numO) {
  byte_t byteBuffer[byteBufferSize];
  std::ofstream dataFileStream(dataFileName(len, numX, numO), std::ios::out|std::ios::binary);

  results.resize(results.size() + (4 - results.size()%4) % 4, (result_t)0); // pad results with 0s so that 4 divides results.size()
  nbyte_t byteBufferWriteIndex = 0;
  for (sindex_t stateIndex = 0; stateIndex < results.size(); stateIndex += 4) { // writing bytes in byte blocks of byteBufferSize
    byte_t writeByte = results[stateIndex] | results[stateIndex+1] << 2 | results[stateIndex+2] << 4 | results[stateIndex+3] << 6;
    byteBuffer[byteBufferWriteIndex] = writeByte;
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
}

std::vector<result_t> DataHandler::loadClass(len_t len, nbit_t numX, nbit_t numO) {
  byte_t byteBuffer[byteBufferSize];
  std::ifstream dataFileStream(dataFileName(len, numX, numO), std::ios::in|std::ios::binary);
  if (dataFileStream.fail()) {
    std::cerr << "error: " << "data file cannot be opened: " << dataFileName(len, numX, numO) << "\n";
    exit(1);
  }

  std::vector<result_t> results;
  while (dataFileStream) { // reading bytes in byte blocks of byteBufferSize
    dataFileStream.read(byteBuffer, byteBufferSize);
    for (nbyte_t i = 0; i < dataFileStream.gcount(); i++) {
      loadByte(byteBuffer[i], results);
    }
  }

  dataFileStream.close();
  return results;
}

void DataHandler::loadByte(byte_t byte, std::vector<result_t> &results) {
  results.push_back((result_t)(byte & resultMask));
  results.push_back((result_t)((byte >> 2) & resultMask));
  results.push_back((result_t)((byte >> 4) & resultMask));
  results.push_back((result_t)((byte >> 6) & resultMask));
}

result_t DataHandler::loadState(len_t len, nbit_t numX, nbit_t numO, sindex_t stateIndex) {
  byte_t byteBuffer[1];
  std::ifstream dataFileStream(dataFileName(len, numX, numO), std::ios::in|std::ios::binary);
  if (dataFileStream.fail()) {
    std::cerr << "error: " << "data file cannot be opened: " << dataFileName(len, numX, numO) << "\n";
    exit(1);
  }

  auto byteIndex = stateIndex/4;
  auto byteOffset = 2 * (stateIndex%4);
  dataFileStream.seekg(byteIndex);
  dataFileStream.read(byteBuffer, 1);
  result_t result = (result_t)((byteBuffer[0] >> byteOffset) & resultMask);

  dataFileStream.close();
  return result;
}

std::string DataHandler::dataFileName(len_t len, nbit_t numX, nbit_t numO) {
  return dataDirPath + "len" + std::to_string(len) + "_x" + std::to_string(numX) + "_o" + std::to_string(numO) + ".dat";
}
