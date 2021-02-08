#pragma once
#include <unordered_map>

void optComputeMain(); // writes output to file

void saveData(int numX, int numO, std::unordered_map<int, int> results);
std::unordered_map<int, int> loadData(int numX, int numO);
std::unordered_map<int, int> valueIteration(int numX, int numO, std::unordered_map<int, int> cache);
