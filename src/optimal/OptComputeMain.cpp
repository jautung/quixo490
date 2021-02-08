#include "OptComputeMain.hpp"
#include <unordered_map>

void optComputeMain() {
  for (int numTiles = 25; numTiles >= 0; numTiles--) {
    for (int numX = 0; numX < numTiles; numX++) {
      int numO = numTiles - numX;
      std::unordered_map<int, int> cache;
      if (numTiles < 25) {
        auto cache1 = loadData(numX, numO+1);
        auto cache2 = loadData(numO, numX+1);
        cache = cache1;
        cache.insert(cache2.begin(), cache2.end());
      }
      auto results = valueIteration(numX, numO, cache);
      saveData(numX, numO, results);
    }
  }
  // TODO
  // auto moves = gameState.allMoves();
  // for (const auto& move: moves) {
  //   std::cout << move;
  // }
  // std::cout << gameState;
  // std::cout << gameState.containsLine(TILE_X) << "\n";
  // std::cout << gameState.containsLine(TILE_O) << "\n";
}

void saveData(int numX, int numO, std::unordered_map<int, int> results) {
  // TODO: probably write raw bytes to file
  return;
}

std::unordered_map<int, int> loadData(int numX, int numO) {
  std::unordered_map<int, int> ret;
  return ret;
}

std::unordered_map<int, int> valueIteration(int numX, int numO, std::unordered_map<int, int> cache) {
  return cache;
}
