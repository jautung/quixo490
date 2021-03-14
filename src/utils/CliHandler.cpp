#include "CliHandler.hpp"
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> CliHandler::readCliParams(std::string str) {
  std::vector<std::string> params;
  std::string delimiter = ",";
  size_t endPos;
  while ((endPos = str.find(delimiter)) != std::string::npos) {
    params.push_back(str.substr(0, endPos));
    str.erase(0, endPos + delimiter.length());
  }
  params.push_back(str);
  return params;
}

int CliHandler::readNonNegIntCliParam(std::string str, std::string descrip) {
  try {
    size_t stoiIndex;
    auto ret = std::stoi(str, &stoiIndex);
    if (stoiIndex != str.length()) {
      std::cerr << "error: " << "non-integer " << descrip << ": " << str << "\n";
      exit(1);
    }
    if (ret < 0) {
      std::cerr << "error: " << "negative " << descrip << ": " << str << "\n";
      exit(1);
    }
    return ret;
  } catch (std::exception const &e) {
    std::cerr << "error: " << "non-integer " << descrip << ": " << str << "\n";
    exit(1);
  }
}
