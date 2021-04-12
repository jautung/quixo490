#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "../utils/DataHandler.hpp"
#include "Players.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

extern std::mt19937 rng;

namespace {
  state_t initState = 0b0;
  long unsigned int maxDepth = 1000;
}

MCTSPlayer::MCTSPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler, int initInitIters, int initPerMoveIters) : Player(initGameStateHandler, initGraphicsHandler) {
  initIters = initInitIters;
  perMoveIters = initPerMoveIters;
}

MCTSPlayer::~MCTSPlayer() {}

move_t MCTSPlayer::selectMove(state_t state, colormode_t colorMode) {
  #if MCTS_CACHE_HIT_CHECK == 1
    prevCacheFrozen.clear();
    for (auto stateInfo : cache) prevCacheFrozen.insert(stateInfo.first);
    prevCacheHits.clear();
  #endif
  for (int i = 0; i < perMoveIters; i++) {
    runIter(state);
  }
  #if MCTS_CACHE_HIT_CHECK == 1
    cacheHits.push_back(prevCacheHits.size());
    cacheSizes.push_back(cache.size());
  #endif
  return selectBestMove(state);
}

void MCTSPlayer::clearCache() {
  cache.clear();
  #if MCTS_CACHE_HIT_CHECK == 1
    cacheHits.clear();
    cacheSizes.clear();
  #endif
}

void MCTSPlayer::initLearn() {
  for (int i = 0; i < initIters; i++) {
    runIter(initState);
  }
}

void MCTSPlayer::runIter(state_t state) {
  std::vector<state_t> traversedStates;
  while (true) {
    traversedStates.push_back(state);
    if (gameStateHandler->containsLine(state, TILE_X)) { // terminal winning
      backPropagate(RESULT_WIN, traversedStates);
      break;
    } else if (gameStateHandler->containsLine(state, TILE_O)) { // terminal losing
      backPropagate(RESULT_LOSS, traversedStates);
      break;
    } else if (traversedStates.size() == maxDepth) {
      backPropagate(RESULT_DRAW, traversedStates);
      break;
    }
    auto childrenStates = getChildrenStates(state);
    auto exploredChildrenStates = std::get<0>(childrenStates);
    auto unexploredChildrenStates = std::get<1>(childrenStates);
    if (unexploredChildrenStates.size() > 0) { // at least one child has not been explored
      std::uniform_int_distribution<int> dist(0, unexploredChildrenStates.size() - 1);
      auto playoutState = *std::next(unexploredChildrenStates.begin(), dist(rng));
      playout(playoutState, traversedStates);
      break;
    } else { // all children have been explored at least once
      auto ucbState = selectBestUcbState(state, exploredChildrenStates);
      state = ucbState;
    }
  }
}

void MCTSPlayer::backPropagate(result_t result, std::vector<state_t> &traversedStates) {
  for (auto rIter = traversedStates.rbegin(); rIter != traversedStates.rend(); rIter++) {
    auto traversedState = *rIter;
    #if MCTS_CACHE_HIT_CHECK == 1
      if (prevCacheFrozen.find(traversedState) != prevCacheFrozen.end()) prevCacheHits.insert(traversedState);
    #endif
    if (result == RESULT_WIN) {
      cache[traversedState] = addTuples(cache[traversedState], std::make_tuple(2, 2));
      result = RESULT_LOSS;
    } else if (result == RESULT_LOSS) {
      cache[traversedState] = addTuples(cache[traversedState], std::make_tuple(2, 0));
      result = RESULT_WIN;
    } else if (result == RESULT_DRAW) {
      cache[traversedState] = addTuples(cache[traversedState], std::make_tuple(2, 1));
    }
  }
}

std::tuple<std::unordered_set<state_t>, std::unordered_set<state_t>> MCTSPlayer::getChildrenStates(state_t state) {
  std::unordered_set<state_t> exploredChildrenStates;
  std::unordered_set<state_t> unexploredChildrenStates;
  auto moves = gameStateHandler->allMoves(state);
  for (auto move : moves) {
    auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
    if (cache.find(childState) != cache.end()) {
      exploredChildrenStates.insert(childState);
    } else {
      unexploredChildrenStates.insert(childState);
    }
  }
  return std::make_tuple(exploredChildrenStates, unexploredChildrenStates);
}

void MCTSPlayer::playout(state_t state, std::vector<state_t> &traversedStates) {
  while (true) {
    traversedStates.push_back(state);
    if (gameStateHandler->containsLine(state, TILE_X)) { // terminal winning
      backPropagate(RESULT_WIN, traversedStates);
      break;
    } else if (gameStateHandler->containsLine(state, TILE_O)) { // terminal losing
      backPropagate(RESULT_LOSS, traversedStates);
      break;
    } else if (traversedStates.size() == maxDepth) {
      backPropagate(RESULT_DRAW, traversedStates);
      break;
    }
    auto moves = gameStateHandler->allMoves(state);
    std::uniform_int_distribution<int> dist(0, moves.size() - 1);
    auto randomMove = moves[dist(rng)];
    state = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, randomMove));
  }
}

state_t MCTSPlayer::selectBestUcbState(state_t state, std::unordered_set<state_t> &exploredChildrenStates) {
  auto numVisits = std::get<0>(cache[state]);
  std::vector<state_t> bestUcbStates;
  double bestUcbValue = 0.0;
  for (auto childState : exploredChildrenStates) {
    auto childInfo = cache[childState];
    auto childNumVisits = std::get<0>(childInfo);
    auto childNumWins = std::get<1>(childInfo);
    double childReward = 1.0 - (double)childNumWins / (double)childNumVisits;
    double childUcbValue = childReward + sqrt(2.0 * log((double)numVisits) / (double)childNumVisits);
    if (childUcbValue > bestUcbValue) {
      bestUcbStates.clear();
      bestUcbStates.push_back(childState);
      bestUcbValue = childUcbValue;
    } else if (childUcbValue == bestUcbValue) {
      bestUcbStates.push_back(childState);
    }
  }
  std::uniform_int_distribution<int> dist(0, bestUcbStates.size() - 1);
  return bestUcbStates[dist(rng)];
}

move_t MCTSPlayer::selectBestMove(state_t state) {
  auto moves = gameStateHandler->allMoves(state);
  std::vector<move_t> bestMoves;
  std::tuple<int, int> bestChildInfo = std::make_tuple(0, 1); // initialized to infinite win rate, since want to select child with lowest win rate
  for (auto move : moves) {
    auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
    auto childCachedInfo = cache.find(childState);
    std::tuple<int, int> childInfo;
    if (childCachedInfo != cache.end()) {
      childInfo = childCachedInfo->second;
    } else { // child has not been searched before, assume baseline 50% win rate
      childInfo = std::make_tuple(2, 1);
    }
    if (worseInfo(childInfo, bestChildInfo)) {
      bestMoves.clear();
      bestMoves.push_back(move);
      bestChildInfo = childInfo;
    } else if (equalInfo(childInfo, bestChildInfo)) {
      bestMoves.push_back(move);
    }
  }
  std::uniform_int_distribution<int> dist(0, bestMoves.size() - 1);
  return bestMoves[dist(rng)];
}

std::tuple<int, int> MCTSPlayer::addTuples(const std::tuple<int, int> &tupleA, const std::tuple<int, int> &tupleB) {
  return std::make_tuple(std::get<0>(tupleA) + std::get<0>(tupleB), std::get<1>(tupleA) + std::get<1>(tupleB));
}

bool MCTSPlayer::equalInfo(const std::tuple<int, int> &infoA, const std::tuple<int, int> &infoB) {
  auto numVisitsA = std::get<0>(infoA);
  auto numWinsA = std::get<1>(infoA);
  auto numVisitsB = std::get<0>(infoB);
  auto numWinsB = std::get<1>(infoB);
  return numWinsA * numVisitsB == numWinsB * numVisitsA; // numWinsA / numVisitsA == numWinsB / numVisitsB
}

bool MCTSPlayer::worseInfo(const std::tuple<int, int> &infoA, const std::tuple<int, int> &infoB) {
  auto numVisitsA = std::get<0>(infoA);
  auto numWinsA = std::get<1>(infoA);
  auto numVisitsB = std::get<0>(infoB);
  auto numWinsB = std::get<1>(infoB);
  return numWinsA * numVisitsB < numWinsB * numVisitsA; // numWinsA / numVisitsA < numWinsB / numVisitsB
}

#if MCTS_CACHE_HIT_CHECK == 1
void MCTSPlayer::printCacheStats(std::string playerName) {
  std::cout << "\n--- MCTS player " << playerName << " cache hit statistics ---\n";
  std::cout << "Turn Num\tCache Size\tCache Hits\tCache Hit Percentage\n";
  for (int turnNum = 0; turnNum < cacheHits.size(); turnNum++) {
    int cacheHit = cacheHits[turnNum];
    int cacheAdded = cacheSizes[turnNum] - (turnNum > 0 ? cacheSizes[turnNum-1] : 0) + cacheHit;
    std::cout << std::setw(8) << turnNum << "\t"
              << std::setw(10) << cacheSizes[turnNum] << "\t"
              << std::setw(10) << cacheHit << "\t"
              << std::setw(20) << std::setiosflags(std::ios::fixed) << std::setprecision(3) << 100.0 * cacheHit / cacheAdded << "\n";
  }
}
#endif
