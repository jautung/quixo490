#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "Players.hpp"
#include <random>
#include <tuple>
#include <unordered_map>
#include <vector>

MCTSPlayer::MCTSPlayer(GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler, int initIters) : Player(initGameStateHandler, initGraphicsHandler) {
  iters = initIters;
}

MCTSPlayer::~MCTSPlayer() {}

move_t MCTSPlayer::selectMove(state_t state, colormode_t colorMode) {
  for (int i = 0; i < iters; i++) {
    runIter(state);
  }
  return selectBestMove(state);
}

void MCTSPlayer::runIter(state_t state) {

}

move_t MCTSPlayer::selectBestMove(state_t state) {
  auto moves = gameStateHandler->allMoves(state);
  std::vector<move_t> bestMoves;
  std::tuple<int, int> bestChildInfo = std::make_tuple(0, 1); // initialized to infinite win rate, since want to select child with lowest win rate
  for (auto move : moves) {
    auto childState = gameStateHandler->swapPlayers(gameStateHandler->makeMove(state, move));
    auto childCachedInfo = cache.find(childState);
    std::tuple<int, int> childInfo;
    if (childCachedInfo == cache.end()) { // child has not been searched before, assume baseline 50% win rate
      childInfo = std::make_tuple(2, 1);
    } else {
      childInfo = childCachedInfo->second;
    }
    if (worseInfo(childInfo, bestChildInfo)) {
      bestMoves.clear();
      bestMoves.push_back(move);
      bestChildInfo = childInfo;
    } else if (equalInfo(childInfo, bestChildInfo)) {
      bestMoves.push_back(move);
    }
  }
  return *std::next(std::begin(bestMoves), rand() % bestMoves.size());
}

bool MCTSPlayer::equalInfo(std::tuple<int, int> infoA, std::tuple<int, int> infoB) {
  auto numVisitsA = std::get<0>(infoA);
  auto numWinsA = std::get<1>(infoA);
  auto numVisitsB = std::get<0>(infoB);
  auto numWinsB = std::get<1>(infoB);
  return numWinsA * numVisitsB == numWinsB * numVisitsA; // numWinsA / numVisitsA == numWinsB / numVisitsB
}

bool MCTSPlayer::worseInfo(std::tuple<int, int> infoA, std::tuple<int, int> infoB) {
  auto numVisitsA = std::get<0>(infoA);
  auto numWinsA = std::get<1>(infoA);
  auto numVisitsB = std::get<0>(infoB);
  auto numWinsB = std::get<1>(infoB);
  return numWinsA * numVisitsB < numWinsB * numVisitsA; // numWinsA / numVisitsA < numWinsB / numVisitsB
}
