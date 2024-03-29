#include "GamePlayHandler.hpp"
#include "GraphicsHandler.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

GamePlayHandler::GamePlayHandler(Player* initPlayerX, Player* initPlayerO, int initTimePauseMs, GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler, std::string initVerbosity) {
  playerX = initPlayerX;
  playerO = initPlayerO;
  timePauseMs = initTimePauseMs;
  gameStateHandler = initGameStateHandler;
  graphicsHandler = initGraphicsHandler;
  verbosity = initVerbosity;
  initTimeX = std::chrono::high_resolution_clock::duration::zero();
  initTimeO = std::chrono::high_resolution_clock::duration::zero();
  runTimeX = std::chrono::high_resolution_clock::duration::zero();
  runTimeO = std::chrono::high_resolution_clock::duration::zero();
}

void GamePlayHandler::startGame(state_t initState) {
  state = initState;
  playerX->clearCache();
  playerO->clearCache();

  auto startTimeX = std::chrono::high_resolution_clock::now();
  playerX->initLearn();
  auto endTimeX = std::chrono::high_resolution_clock::now();
  initTimeX += endTimeX - startTimeX;

  auto startTimeO = std::chrono::high_resolution_clock::now();
  playerO->initLearn();
  auto endTimeO = std::chrono::high_resolution_clock::now();
  initTimeO += endTimeO - startTimeO;
}

winner_t GamePlayHandler::playTurn() {
  displayGameState();

  auto startTimeX = std::chrono::high_resolution_clock::now();
  auto move = playerX->selectMove(state);
  auto endTimeX = std::chrono::high_resolution_clock::now();
  runTimeX += endTimeX - startTimeX;
  if (!dynamic_cast<InteractivePlayer*>(playerX)) { // pause for all except interactive player
    std::this_thread::sleep_for(std::chrono::milliseconds(timePauseMs));
  }
  if (verbosity == "all") {
    std::cout << "X's Move: ";
    gameStateHandler->moveHandler->print(move);
  }
  state = gameStateHandler->makeMove(state, move);

  if (gameStateHandler->containsLine(state, TILE_O)) {
    displayGameState();
    return WINNER_O;
  } else if (gameStateHandler->containsLine(state, TILE_X)) {
    displayGameState();
    return WINNER_X;
  }

  displayGameState();

  state = gameStateHandler->swapPlayers(state);
  auto startTimeO = std::chrono::high_resolution_clock::now();
  move = playerO->selectMove(state, COLOR_FLIP);
  auto endTimeO = std::chrono::high_resolution_clock::now();
  runTimeO += endTimeO - startTimeO
  ;
  if (!dynamic_cast<InteractivePlayer*>(playerO)) { // pause for all except interactive player
    std::this_thread::sleep_for(std::chrono::milliseconds(timePauseMs));
  }
  if (verbosity == "all") {
    std::cout << "O's Move: ";
    gameStateHandler->moveHandler->print(move);
  }
  state = gameStateHandler->makeMove(state, move);
  state = gameStateHandler->swapPlayers(state);

  if (gameStateHandler->containsLine(state, TILE_X)) {
    displayGameState();
    return WINNER_X;
  } else if (gameStateHandler->containsLine(state, TILE_O)) {
    displayGameState();
    return WINNER_O;
  }

  return WINNER_UNKNOWN;
}

winner_t GamePlayHandler::playNTurns(int nTurns, int& nTurnsPlayed) {
  for (int turnNum = 0; turnNum < nTurns; turnNum++) {
    auto winner = playTurn();
    if (winner == WINNER_X || winner == WINNER_O) {
      nTurnsPlayed = turnNum;
      return winner;
    }
  }
  displayGameState();
  nTurnsPlayed = nTurns;
  return WINNER_UNKNOWN;
}

winner_t GamePlayHandler::playTillEnd(int& nTurnsPlayed) {
  for (int turnNum = 0; turnNum < INT_MAX; turnNum++) {
    auto winner = playTurn();
    if (winner == WINNER_X || winner == WINNER_O) {
      nTurnsPlayed = turnNum;
      return winner;
    }
  }
  nTurnsPlayed = INT_MAX;
  while (true) {
    auto winner = playTurn();
    if (winner == WINNER_X || winner == WINNER_O) {
      return winner;
    }
  }
}

void GamePlayHandler::displayGameState() {
  if (!graphicsHandler) {
    if (verbosity == "all") {
      gameStateHandler->print(state);
    }
  } else {
    if (verbosity == "all" || verbosity == "default") {
      graphicsHandler->drawBoard(state);
    }
  }
}
