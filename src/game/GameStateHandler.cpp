#include "GameStateHandler.hpp"
#include <iostream>
#include <random>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

extern std::mt19937 rng;

namespace {
  nbit_t stateNBits = 64;
  nbit_t halfStateNBits = stateNBits/2;
  state_t halfStateMask = 0b11111111111111111111111111111111;
  move_t moveMainMask = 0b111111111111;
}

void MoveHandler::print(move_t move) {
  auto dir = getDir(move);
  auto row = getRow(move);
  auto col = getCol(move);
  auto dirrev = getDirRev(move);
  if (dir == DIR_LEFT) {
    std::cout << "LEFT " << (dirrev == DIR_REV ? "REVERSED " : "") << "on row " << +row << " (taking tile from column " << +col << ")\n";
  } else if (dir == DIR_RIGHT) {
    std::cout << "RIGHT " << (dirrev == DIR_REV ? "REVERSED " : "") << "on row " << +row << " (taking tile from column " << +col << ")\n";
  } else if (dir == DIR_DOWN) {
    std::cout << "DOWN " << (dirrev == DIR_REV ? "REVERSED " : "") << "on column " << +col << " (taking tile from row " << +row << ")\n";
  } else if (dir == DIR_UP) {
    std::cout << "UP " << (dirrev == DIR_REV ? "REVERSED " : "") << "on column " << +col << " (taking tile from row " << +row << ")\n";
  } else if (dir == DIR_UNDEFINED) {
    std::cout << "UNDEFINED\n";
  }
}

GameStateHandler::GameStateHandler(len_t initLen) {
  len = initLen;
  moveHandler = new MoveHandler();

  // default masks
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

  // populating all potential moves cache
  for (bindex_t i = 0; i < len; i++) {
    for (bindex_t j = 0; j < len; j++) {
      if (i == 0 || i == len-1 || j == 0 || j == len-1) {
        if (j != len-1) {
          allPotentialMovesCache.push_back(moveHandler->create(DIR_LEFT, i, j, MKIND_PLUS));
          allPotentialMovesCache.push_back(moveHandler->create(DIR_LEFT, i, j, MKIND_ZERO));
        }
        if (j != 0) {
          allPotentialMovesCache.push_back(moveHandler->create(DIR_RIGHT, i, j, MKIND_PLUS));
          allPotentialMovesCache.push_back(moveHandler->create(DIR_RIGHT, i, j, MKIND_ZERO));
        }
        if (i != 0) {
          allPotentialMovesCache.push_back(moveHandler->create(DIR_DOWN, i, j, MKIND_PLUS));
          allPotentialMovesCache.push_back(moveHandler->create(DIR_DOWN, i, j, MKIND_ZERO));
        }
        if (i != len-1) {
          allPotentialMovesCache.push_back(moveHandler->create(DIR_UP, i, j, MKIND_PLUS));
          allPotentialMovesCache.push_back(moveHandler->create(DIR_UP, i, j, MKIND_ZERO));
        }
      }
    }
  }

  // populating make move cache
  for (auto move : allPotentialMovesCache) {
    auto kind = moveHandler->getKind(move);
    if (kind == MKIND_ZERO) {continue;}

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

    state_t oldTile = (0b1 << (len*row + col)); // position of the old tile picked up
    oldTile <<= halfStateNBits;

    state_t mask = 0b0; // creating the mask
    if (dir == DIR_LEFT || dir == DIR_RIGHT) {
      mask = (rowMaskFull >> (len-maskNumBits)) << maskStartBit;
    } else if (dir == DIR_DOWN || dir == DIR_UP) {
      mask = (colMaskFull >> (len * (len-maskNumBits))) << maskStartBit;
    }
    mask = (mask << halfStateNBits) | mask; // mask both X's and O's

    makeMoveCache[move & moveMainMask] = std::make_tuple(mask, newTile, oldTile);
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

state_t GameStateHandler::setTile(state_t state, bindex_t row, bindex_t col, tile_t tile) {
  state_t oMask = 0b1 << (len*row + col);
  if (tile == TILE_O) {
    state |= oMask;
  } else {
    state &= ~oMask;
  }
  state_t xMask = oMask <<= halfStateNBits;
  if (tile == TILE_X) {
    state |= xMask;
  } else {
    state &= ~xMask;
  }
  return state;
}

std::vector<move_t> GameStateHandler::allMoves(state_t state) {
  std::vector<move_t> moves;
  std::unordered_set<int> movesIndices;
  allMovesAux(state, moves, movesIndices);
  return moves;
}

std::unordered_set<int> GameStateHandler::allMovesIndices(state_t state) {
  std::vector<move_t> moves;
  std::unordered_set<int> movesIndices;
  allMovesAux(state, moves, movesIndices);
  return movesIndices;
}

void GameStateHandler::allMovesAux(state_t state, std::vector<move_t> &moves, std::unordered_set<int> &movesIndices) {
  for (size_t moveIndex = 0; moveIndex < allPotentialMovesCache.size(); moveIndex++) {
    auto move = allPotentialMovesCache[moveIndex];
    auto i = moveHandler->getRow(move);
    auto j = moveHandler->getCol(move);
    auto tile = getTile(state, i, j);
    auto kind = moveHandler->getKind(move);
    if ((tile == TILE_EMPTY && kind == MKIND_PLUS) || (tile == TILE_X && kind == MKIND_ZERO)) {
      moves.push_back(move);
      movesIndices.insert(moveIndex);
    }
  }
}

std::vector<state_t> GameStateHandler::allPlusParents(state_t state) {
  return allParentsAux(state, MKIND_PLUS);
}

std::vector<state_t> GameStateHandler::allZeroParents(state_t state) {
  return allParentsAux(state, MKIND_ZERO);
}

std::vector<state_t> GameStateHandler::allParentsAux(state_t state, mkind_t kind) {
  state = swapPlayers(state);
  std::unordered_set<state_t> parentsSet;
  std::vector<state_t> parents;
  for (bindex_t i = 0; i < len; i++) {
    for (bindex_t j = 0; j < len; j++) {
      if (i == 0 || i == len-1 || j == 0 || j == len-1) {
        auto tile = getTile(state, i, j);
        if (tile == TILE_X) {
          if (j == 0 || j == len-1) {
            auto parent = makeMove(state, moveHandler->create(j == 0 ? DIR_RIGHT : DIR_LEFT, i, j == 0 ? len-1 : 0, kind, DIR_REV));
            if (parentsSet.find(parent) == parentsSet.end()) {parentsSet.insert(parent); parents.push_back(parent);}
            if (i == 0 || i == len-1) {
              for (bindex_t jFrom = 1; jFrom < len-1; jFrom++) {
                auto parent = makeMove(state, moveHandler->create(j == 0 ? DIR_RIGHT : DIR_LEFT, i, jFrom, kind, DIR_REV));
                if (parentsSet.find(parent) == parentsSet.end()) {parentsSet.insert(parent); parents.push_back(parent);}
              }
            }
          }
          if (i == 0 || i == len-1) {
            auto parent = makeMove(state, moveHandler->create(i == 0 ? DIR_DOWN : DIR_UP, i == 0 ? len-1 : 0, j, kind, DIR_REV));
            if (parentsSet.find(parent) == parentsSet.end()) {parentsSet.insert(parent); parents.push_back(parent);}
            if (j == 0 || j == len-1) {
              for (bindex_t iFrom = 1; iFrom < len-1; iFrom++) {
                auto parent = makeMove(state, moveHandler->create(i == 0 ? DIR_DOWN : DIR_UP, iFrom, j, kind, DIR_REV));
                if (parentsSet.find(parent) == parentsSet.end()) {parentsSet.insert(parent); parents.push_back(parent);}
              }
            }
          }
        }
      }
    }
  }
  return parents;
}

state_t GameStateHandler::makeMove(state_t state, move_t move) {
  auto cached = makeMoveCache[move & moveMainMask];
  state_t mask = std::get<0>(cached);
  state_t newTile = std::get<1>(cached);
  state_t oldTile = std::get<2>(cached);
  auto dir = moveHandler->getDir(move);
  auto dirrev = moveHandler->getDirRev(move);
  auto kind = moveHandler->getKind(move);
  if (dir == DIR_LEFT) {
    if (dirrev == DIR_NOREV) {
      return (((state & mask) >> 1) & mask) | (state & ~mask) | newTile;
    } else if (dirrev == DIR_REV) {
      return (((state & mask) << 1) & mask) | (state & ~mask) | (kind == MKIND_ZERO ? oldTile : 0b0);
    }
  } else if (dir == DIR_RIGHT) {
    if (dirrev == DIR_NOREV) {
      return (((state & mask) << 1) & mask) | (state & ~mask) | newTile;
    } else if (dirrev == DIR_REV) {
      return (((state & mask) >> 1) & mask) | (state & ~mask) | (kind == MKIND_ZERO ? oldTile : 0b0);
    }
  } else if (dir == DIR_DOWN) {
    if (dirrev == DIR_NOREV) {
      return (((state & mask) << len) & mask) | (state & ~mask) | newTile;
    } else if (dirrev == DIR_REV) {
      return (((state & mask) >> len) & mask) | (state & ~mask) | (kind == MKIND_ZERO ? oldTile : 0b0);
    }
  } else if (dir == DIR_UP) {
    if (dirrev == DIR_NOREV) {
      return (((state & mask) >> len) & mask) | (state & ~mask) | newTile;
    } else if (dirrev == DIR_REV) {
      return (((state & mask) << len) & mask) | (state & ~mask) | (kind == MKIND_ZERO ? oldTile : 0b0);
    }
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
  return __builtin_popcount((state >> halfStateNBits) & halfStateMask);
}

nbit_t GameStateHandler::getNumO(state_t state) {
  return __builtin_popcount(state & halfStateMask);
}

bindex_t GameStateHandler::numInRow(state_t state, tile_t tileType, bindex_t rowIndex) {
  nbit_t stateShift = tileType == TILE_X ? halfStateNBits : 0;
  state >>= stateShift;
  state &= halfStateMask;
  state_t mask = rowMaskFull << (rowIndex * len);
  return __builtin_popcount(state & mask);
}

bindex_t GameStateHandler::numInCol(state_t state, tile_t tileType, bindex_t colIndex) {
  nbit_t stateShift = tileType == TILE_X ? halfStateNBits : 0;
  state >>= stateShift;
  state &= halfStateMask;
  state_t mask = colMaskFull << colIndex;
  return __builtin_popcount(state & mask);
}

bindex_t GameStateHandler::numInDiag1(state_t state, tile_t tileType) {
  nbit_t stateShift = tileType == TILE_X ? halfStateNBits : 0;
  state >>= stateShift;
  state &= halfStateMask;
  state_t mask = diag1MaskFull;
  return __builtin_popcount(state & mask);
}

bindex_t GameStateHandler::numInDiag2(state_t state, tile_t tileType) {
  nbit_t stateShift = tileType == TILE_X ? halfStateNBits : 0;
  state >>= stateShift;
  state &= halfStateMask;
  state_t mask = diag2MaskFull;
  return __builtin_popcount(state & mask);
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

state_t GameStateHandler::genRandomNonTerminalState() {
  state_t state;
  do {
    state = 0b0;
    for (bindex_t i = 0; i < len; i++) {
      for (bindex_t j = 0; j < len; j++) {
        std::uniform_int_distribution<int> dist(0, 2);
        switch (dist(rng)) {
          case 0:
            state = setTile(state, i, j, TILE_X);
            break;
          case 1:
            state = setTile(state, i, j, TILE_O);
            break;
          case 2:
            state = setTile(state, i, j, TILE_EMPTY);
            break;
        }
      }
    }
  } while (containsLine(state, TILE_X) || containsLine(state, TILE_O));
  return state;
}
