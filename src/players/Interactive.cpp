#include "../game/GameStateHandler.hpp"
#include "../game/GraphicsHandler.hpp"
#include "Players.hpp"
#include <iostream>

move_t InteractivePlayer::selectMove(state_t state, int turnNum, colormode_t colorMode) {
  if (!graphicsHandler) {
    auto moves = gameStateHandler->allMoves(state);
    std::cout << "Choose one of:\n";
    int i = 0;
    for (auto moveIter = moves.begin(); moveIter != moves.end(); moveIter++, i++) {
      std::cout << i << " - ";
      gameStateHandler->moveHandler->print(*moveIter);
    }

    int choice;
    while (true) {
      std::cout << "Choice: ";
      if (std::cin >> choice && !std::cin.fail() && choice >= 0 && choice < i) {
        break;
      } else {
        std::cout << "Invalid choice\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }

    i = 0;
    for (auto moveIter = moves.begin(); moveIter != moves.end(); moveIter++, i++) {
      if (i == choice) {
        return *moveIter;
      }
    }
    return gameStateHandler->moveHandler->create(DIR_UNDEFINED, 0, 0);
  } else {
    return graphicsHandler->drawBoardGetMove(state, colorMode);
  }
}
