#include "GameStateHandler.hpp"
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace {
  nbit_t stateNBits = 64;
  nbit_t halfStateNBits = stateNBits/2;
  state_t xStateMask = 0b1111111111111111111111111111111100000000000000000000000000000000;
  state_t oStateMask = 0b0000000000000000000000000000000011111111111111111111111111111111;
  move_t moveFieldMask = 0b1111;
  move_t moveMainMask = 0b111111111111;
}

move_t MoveHandler::create(dir_t dir, bindex_t rowIndex, bindex_t colIndex, mkind_t moveKind) {
  return dir | rowIndex << 4 | colIndex << 8 | moveKind << 12;
}

dir_t MoveHandler::getDir(move_t move) {
  return (dir_t)(move & moveFieldMask);
}

bindex_t MoveHandler::getRow(move_t move) {
  return (bindex_t)((move >> 4) & moveFieldMask);
}

bindex_t MoveHandler::getCol(move_t move) {
  return (bindex_t)((move >> 8) & moveFieldMask);
}

mkind_t MoveHandler::getKind(move_t move) {
  return (mkind_t)((move >> 12) & moveFieldMask);
}

void MoveHandler::print(move_t move) {
  auto dir = getDir(move);
  auto row = getRow(move);
  auto col = getCol(move);
  if (dir == DIR_LEFT) {
    std::cout << "LEFT on row " << +row << " (taking tile from column " << +col << ")\n";
  } else if (dir == DIR_RIGHT) {
    std::cout << "RIGHT on row " << +row << " (taking tile from column " << +col << ")\n";
  } else if (dir == DIR_DOWN) {
    std::cout << "DOWN on column " << +col << " (taking tile from row " << +row << ")\n";
  } else if (dir == DIR_UP) {
    std::cout << "UP on column " << +col << " (taking tile from row " << +row << ")\n";
  } else if (dir == DIR_UNDEFINED) {
    std::cout << "UNDEFINED\n";
  }
}

GameStateHandler::GameStateHandler(len_t initLen) {
  len = initLen;
  moveHandler = new MoveHandler();

  rowMaskFull = 0b0;
  for (nbit_t i = 0; i < len; i++) {
    rowMaskFull <<= 1;
    rowMaskFull |= 0b1;
  }
  colMaskFull = 0b0;
  for (nbit_t i = 0; i < len; i++) {
    colMaskFull <<= len;
    colMaskFull |= 0b1;
  }
  diag1MaskFull = 0b0;
  for (nbit_t i = 0; i < len; i++) {
    diag1MaskFull <<= len+1;
    diag1MaskFull |= 0b1;
  }
  diag2MaskFull = 0b0;
  for (nbit_t i = 0; i < len; i++) {
    diag2MaskFull <<= len-1;
    diag2MaskFull |= 0b1;
  }
  diag2MaskFull <<= len-1;

  auto moves = allMoves(0b0); // empty board has all possible moves
  for (auto move : moves) {
    auto dir = moveHandler->getDir(move);
    auto row = moveHandler->getRow(move);
    auto col = moveHandler->getCol(move);

    nbit_t maskNumBits = 0; // number of tiles moved
    nbit_t maskStartBit = 0; // start bit of the masking sequence (most top or most left tile moved)
    state_t newTile = 0b0; // position of the new tile inserted

    if (dir == DIR_LEFT) {
      maskNumBits = len - col;
      maskStartBit = len*row + col;
      newTile = 0b1 << (len*row + len-1);
    } else if (dir == DIR_RIGHT) {
      maskNumBits = 1 + col;
      maskStartBit = len*row;
      newTile = 0b1 << (len*row);
    } else if (dir == DIR_DOWN) {
      maskNumBits = 1 + row;
      maskStartBit = col;
      newTile = 0b1 << (col);
    } else if (dir == DIR_UP) {
      maskNumBits = len - row;
      maskStartBit = len*row + col;
      newTile = 0b1 << (len*(len-1) + col);
    }
    newTile <<= halfStateNBits; // the new tile is inserted as an X

    state_t mask = 0b0; // creating the mask
    if (dir == DIR_LEFT || dir == DIR_RIGHT) {
      mask = (rowMaskFull >> (len-maskNumBits)) << maskStartBit;
    } else if (dir == DIR_DOWN || dir == DIR_UP) {
      mask = (colMaskFull >> (len * (len-maskNumBits))) << maskStartBit;
    }
    mask = (mask << halfStateNBits) | mask; // mask both X's and O's

    makeMoveCache[move & moveMainMask] = std::make_tuple(mask, newTile);
  }
}

GameStateHandler::~GameStateHandler() {
  delete moveHandler;
}

tile_t GameStateHandler::getTile(state_t state, bindex_t row, bindex_t col) {
  state_t mask = 0b1 << (len*row + col);
  if ((state & mask) == mask) {
    return TILE_O;
  }
  mask <<= halfStateNBits;
  if ((state & mask) == mask) {
    return TILE_X;
  }
  return TILE_EMPTY;
}

std::vector<move_t> GameStateHandler::allMoves(state_t state) {
  std::vector<move_t> moves;
  moves.reserve(12*len-16); // maximum number of possible moves
  for (bindex_t i = 0; i < len; i++) {
    for (bindex_t j = 0; j < len; j++) {
      if (i == 0 || i == len-1 || j == 0 || j == len-1) {
        auto tile = getTile(state, i, j);
        if (tile == TILE_X || tile == TILE_EMPTY) {
          if (j != len) {
            moves.push_back(moveHandler->create(DIR_LEFT, i, j, tile == TILE_EMPTY ? MKIND_PLUS : MKIND_ZERO));
          }
          if (j != 0) {
            moves.push_back(moveHandler->create(DIR_RIGHT, i, j, tile == TILE_EMPTY ? MKIND_PLUS : MKIND_ZERO));
          }
          if (i != 0) {
            moves.push_back(moveHandler->create(DIR_DOWN, i, j, tile == TILE_EMPTY ? MKIND_PLUS : MKIND_ZERO));
          }
          if (i != len) {
            moves.push_back(moveHandler->create(DIR_UP, i, j, tile == TILE_EMPTY ? MKIND_PLUS : MKIND_ZERO));
          }
        }
      }
    }
  }
  return moves;
}

state_t GameStateHandler::makeMove(state_t state, move_t move) {
  auto cached = makeMoveCache[move & moveMainMask];
  state_t mask = std::get<0>(cached);
  state_t newTile = std::get<1>(cached);
  auto dir = moveHandler->getDir(move);
  if (dir == DIR_LEFT) {
    return (((state & mask) >> 1) & mask) | newTile | (state & ~mask);
  } else if (dir == DIR_RIGHT) {
    return (((state & mask) << 1) & mask) | newTile | (state & ~mask);
  } else if (dir == DIR_DOWN) {
    return (((state & mask) << len) & mask) | newTile | (state & ~mask);
  } else if (dir == DIR_UP) {
    return (((state & mask) >> len) & mask) | newTile | (state & ~mask);
  }
  return state; // dummy
}

state_t GameStateHandler::swapPlayers(state_t state) {
  return state << halfStateNBits | state >> halfStateNBits;
}

bool GameStateHandler::containsLine(state_t state, tile_t tileType) {
  nbit_t maskShift = tileType == TILE_X ? halfStateNBits : 0;
  state_t mask = rowMaskFull << maskShift;
  for (bindex_t i = 0; i < len; i++, mask <<= len) {
    if ((state & mask) == mask) {
      return true;
    }
  }
  mask = colMaskFull << maskShift;
  for (bindex_t j = 0; j < len; j++, mask <<= 1) {
    if ((state & mask) == mask) {
      return true;
    }
  }
  mask = diag1MaskFull << maskShift;
  if ((state & mask) == mask) {
    return true;
  }
  mask = diag2MaskFull << maskShift;
  if ((state & mask) == mask) {
    return true;
  }
  return false;
}

nbit_t GameStateHandler::getNumX(state_t state) {
  return __builtin_popcount(state & xStateMask);
}

nbit_t GameStateHandler::getNumO(state_t state) {
  return __builtin_popcount(state & oStateMask);
}

void GameStateHandler::print(state_t state) {
  for (bindex_t i = 0; i < len; i++) {
    for (bindex_t j = 0; j < len; j++) {
      auto tile = getTile(state, i, j);
      if (tile == TILE_X) {
        std::cout << "X";
      } else if (tile == TILE_O) {
        std::cout << "O";
      } else if (tile == TILE_EMPTY) {
        std::cout << ".";
      }
    }
    std::cout << "\n";
  }
}
