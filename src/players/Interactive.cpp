#include "../game/GameState.hpp"
#include "../game/Graphics.hpp"
#include "Players.hpp"
#include <iostream>

InteractivePlayer::InteractivePlayer(Graphics* initGraphics) {
  graphics = initGraphics;
}

Move InteractivePlayer::selectMove(const GameState* gameState) {
  if (!graphics) {
    auto moves = gameState->allMoves();
    std::cout << "Choose one of:\n";
    int i = 0;
    for (auto moveIter = moves.begin(); moveIter != moves.end(); moveIter++, i++) {
      std::cout << i << " - " << *moveIter << "\n";
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
    return Move(DIR_LEFT, 0, 0); // dummy
  } else {
    return graphics->drawBoardGetInput(gameState);
  }
}
