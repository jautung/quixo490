#include "MemoryChecker.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

// Adapted from https://www.tutorialspoint.com/how-to-get-memory-usage-at-runtime-using-cplusplus
void MemoryChecker::checkVmRss(std::string prefix) {
  std::ifstream statStream("/proc/self/stat", std::ios_base::in);
  std::string tmp;
  unsigned long vsize;
  long rss;
  statStream >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> vsize >> rss;
  statStream.close();

  long pageSizeKb = sysconf(_SC_PAGE_SIZE) / 1024; // x86-64 is configured to use 2MB pages
  double vmUsage = vsize / 1024.0;
  double residentSetSize = rss * pageSizeKb;

  std::cerr << "Memory [" << prefix << "]: VM=" << vmUsage << ", RSS=" << residentSetSize << "\n";
}

void MemoryChecker::checkVector(void* beginPointer, void* endPointer, std::string prefix) {
  std::cerr << "Memory [Vector " << prefix << " loaded]: " << (uint8_t*)endPointer - (uint8_t*)beginPointer << " bytes\n";
}
