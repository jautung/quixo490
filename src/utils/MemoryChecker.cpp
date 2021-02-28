#include "MemoryChecker.hpp"
#include <fstream>
#include <string>
#include <unistd.h>

// Adapted from https://www.tutorialspoint.com/how-to-get-memory-usage-at-runtime-using-cplusplus
void MemoryChecker::checkUsage(double* vmUsage, double* residentSetSize) {
  *vmUsage = 0.0;
  *residentSetSize = 0.0;

  std::ifstream statStream("/proc/self/stat", std::ios_base::in);
  std::string tmp;
  unsigned long vsize;
  long rss;
  statStream >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> vsize >> rss;
  statStream.close();

  long pageSizeKb = sysconf(_SC_PAGE_SIZE) / 1024; // x86-64 is configured to use 2MB pages
  *vmUsage = vsize / 1024.0;
  *residentSetSize = rss * pageSizeKb;
}
