#pragma once
#include <string>

class MemoryChecker {
  public:
    void checkVmRss(std::string prefix = "");
    void checkVector(void* beginPointer, void* endPointer, std::string prefix = "");
};
