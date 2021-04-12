#pragma once
#include <string>
#include <vector>

class CliHandler {
  public:
    std::vector<std::string> readCliParams(std::string str);
    int readNonNegIntCliParam(std::string str, std::string descrip);
    double readFracDoubleCliParam(std::string str, std::string descrip);
};
