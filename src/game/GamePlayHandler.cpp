#include "GamePlayHandler.hpp"
#include "GraphicsHandler.hpp"
#include <chrono>
#include <iostream>
#include <thread>

GamePlayHandler::GamePlayHandler(Player* initPlayerX, Player* initPlayerO, int initTimePauseMs, GameStateHandler* initGameStateHandler, GraphicsHandler* initGraphicsHandler, bool initSilent) {
  playerX = initPlayerX;
  playerO = initPlayerO;
  timePauseMs = initTimePauseMs;
  gameStateHandler = initGameStateHandler;
  graphicsHandler = initGraphicsHandler;
  silent = initSilent;
}

void GamePlayHandler::startGame(state_t initState) {
  state = initState;
}

winner_t GamePlayHandler::playTurn() {
  displayGameState();

  auto move = playerX->selectMove(state);
  if (!dynamic_cast<InteractivePlayer*>(playerX)) { // pause for all except interactive player
    std::this_thread::sleep_for(std::chrono::milliseconds(timePauseMs));
  }
  if (!silent) {
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
  move = playerO->selectMove(state, COLOR_FLIP);
  if (!dynamic_cast<InteractivePlayer*>(playerO)) { // pause for all except interactive player
    std::this_thread::sleep_for(std::chrono::milliseconds(timePauseMs));
  }
  if (!silent) {
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

winner_t GamePlayHandler::playNTurns(int nTurns) {
  for (int i = 0; i < nTurns; i++) {
    auto winner = playTurn();
    if (winner == WINNER_X || winner == WINNER_O) {
      return winner;
    }
  }
  displayGameState();
  return WINNER_UNKNOWN;
}

winner_t GamePlayHandler::playTillEnd() {
  while (true) {
    auto winner = playTurn();
    if (winner == WINNER_X || winner == WINNER_O) {
      return winner;
    }
  }
}

void GamePlayHandler::displayGameState() {
  if (!silent) {
    if (!graphicsHandler) {
      gameStateHandler->print(state);
    } else {
      graphicsHandler->drawBoard(state);
    }
  }
}
