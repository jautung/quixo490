#include "GameState.hpp"
#include <iostream>

namespace {
  int halfBits = 32;
  state_t tileMask = 0b1;
  state_t rowMaskFull = 0b11111;
  state_t colMaskFull = 0b100001000010000100001;
  state_t diag1MaskFull = 0b1000001000001000001000001;
  state_t diag2MaskFull = 0b0000100010001000100010000;
}

Move::Move(dir_t initDir, index_t initSlideIndex, index_t initCrossIndex) {
  dir = initDir;
  slideIndex = initSlideIndex;
  crossIndex = initCrossIndex;
}

std::ostream &operator<<(std::ostream &os, const Move& move) {
  if (move.dir == DIR_LEFT) {
    return os << "LEFT-" << move.slideIndex << " (" << move.crossIndex << ")\n";
  } else if (move.dir == DIR_RIGHT) {
    return os << "RIGHT-" << move.slideIndex << " (" << move.crossIndex << ")\n";
  } else if (move.dir == DIR_DOWN) {
    return os << "DOWN-" << move.slideIndex << " (" << move.crossIndex << ")\n";
  } else {
    return os << "UP-" << move.slideIndex << " (" << move.crossIndex << ")\n";
  }
}

GameState::GameState(state_t* initState) {
  // TODO: handle different-sized boards appropriately
  if (initState == NULL) {
    state = 0; // no Xs or Os
  } else {
    state = *initState;
  }
}

tile_t GameState::getTile(index_t row, index_t col) const {
  state_t mask = tileMask << (5 * row + col);
  if ((state & mask) == mask) {
    return TILE_O;
  }
  mask <<= halfBits;
  if ((state & mask) == mask) {
    return TILE_X;
  }
  return TILE_EMPTY;
}

std::unordered_set<Move, Move::Hash> GameState::allMoves() const {
  std::unordered_set<Move, Move::Hash> moves;
  for (index_t i = 0; i < 5; i++) {
    for (index_t j = 0; j < 5; j++) {
      if (i == 0 || i == 4 || j == 0 || j == 4) {
        auto tile = getTile(i, j);
        if (tile == TILE_X || tile == TILE_EMPTY) {
          if (j != 4) {
            moves.insert(Move(DIR_LEFT, i, j));
          }
          if (j != 0) {
            moves.insert(Move(DIR_RIGHT, i, j));
          }
          if (i != 0) {
            moves.insert(Move(DIR_DOWN, j, i));
          }
          if (i != 4) {
            moves.insert(Move(DIR_UP, j, i));
          }
        }
      }
    }
  }
  return moves;
}

void GameState::makeMove(Move move) {
  // recall that X is always the active player by abuse of notation
  // TODO: if speed-up is necessary, possibly move computation of mask and newTile to an initialization step, and simply store a unordered_map in memory from Move to mask and newTile
  int maskNumBits = (move.dir == DIR_LEFT || move.dir == DIR_UP) ? 5 - move.crossIndex : 1 + move.crossIndex; // number of tiles moved
  int maskStartBit; // start bit of the masking sequence (most top or left tile moved)
  state_t newTile; // position of the new tile inserted
  if (move.dir == DIR_LEFT) {
    maskStartBit = 5 * move.slideIndex + move.crossIndex;
    newTile = tileMask << (5 * move.slideIndex + 4);
  } else if (move.dir == DIR_RIGHT) {
    maskStartBit = 5 * move.slideIndex;
    newTile = tileMask << (5 * move.slideIndex);
  } else if (move.dir == DIR_DOWN) {
    maskStartBit = move.slideIndex;
    newTile = tileMask << (move.slideIndex);
  } else {
    maskStartBit = 5 * move.crossIndex + move.slideIndex;
    newTile = tileMask << (5 * 4 + move.slideIndex);
  }
  newTile <<= halfBits; // new tile is inserted as an X

  state_t mask; // creating the mask
  if (move.dir == DIR_LEFT || move.dir == DIR_RIGHT) {
    mask = (rowMaskFull >> (5 - maskNumBits)) << maskStartBit;
  } else {
    mask = (colMaskFull >> (5 * (5 - maskNumBits))) << maskStartBit;
  }
  mask = (mask << halfBits) | mask; // mask both X's and O's

  if (move.dir == DIR_LEFT) { // applying the shift using the mask and newTile
    state = (((state & mask) >> 1) & mask) | newTile | (state & ~mask);
  } else if (move.dir == DIR_RIGHT) {
    state = (((state & mask) << 1) & mask) | newTile | (state & ~mask);
  } else if (move.dir == DIR_DOWN) {
    state = (((state & mask) << 5) & mask) | newTile | (state & ~mask);
  } else {
    state = (((state & mask) >> 5) & mask) | newTile | (state & ~mask);
  }
}

void GameState::swapPlayers() {
  state = state << halfBits | state >> halfBits;
}

bool GameState::containsLine(tile_t tileType) const {
  state_t mask = rowMaskFull << ((tileType == TILE_X) ? halfBits : 0);
  for (index_t i = 0; i < 5; i++, mask <<= 5) { // along rows
    if ((state & mask) == mask) {
      return true;
    }
  }
  mask = colMaskFull << ((tileType == TILE_X) ? halfBits : 0);
  for (index_t j = 0; j < 5; j++, mask <<= 1) { // along columns
    if ((state & mask) == mask) {
      return true;
    }
  }
  mask = diag1MaskFull << ((tileType == TILE_X) ? halfBits : 0);
  if ((state & mask) == mask) {
    return true;
  }
  mask = diag2MaskFull << ((tileType == TILE_X) ? halfBits : 0);
  if ((state & mask) == mask) {
    return true;
  }
  return false;
}

std::ostream &operator<<(std::ostream &os, const GameState& gameState) {
  // TODO: make a GUI for this
  for (index_t i = 0; i < 5; i++) {
    for (index_t j = 0; j < 5; j++) {
      auto tile = gameState.getTile(i, j);
      if (tile == TILE_X) {
        os << "X";
      } else if (tile == TILE_O) {
        os << "O";
      } else {
        os << ".";
      }
    }
    os << "\n";
  }
  return os;
}
