#pragma once
#include <cstdint>
#include <tuple>
#include <unordered_map>
#include <vector>

typedef uint8_t nbit_t;   // number of bits in a number
typedef uint64_t state_t; // representation as presented in the referenced paper

typedef uint8_t len_t;    // number of tiles per side
typedef int8_t bindex_t;  // index for rows and columns

enum tile_t : uint8_t {   // type of tile
  TILE_X,
  TILE_O,
  TILE_EMPTY
};

enum dir_t : uint8_t {    // direction
  DIR_LEFT,
  DIR_RIGHT,
  DIR_DOWN,
  DIR_UP,
  DIR_UNDEFINED
};

enum dirrev_t : uint8_t { // direction reversal
  DIR_NOREV,
  DIR_REV
};

enum mkind_t : uint8_t {
  MKIND_ZERO,
  MKIND_PLUS,
  MKIND_UNDEFINED
};

typedef uint16_t move_t;  // from LSB to MSB: 4 bits direction, 4 bits row index, 4 bits column index, 3 bits move kind, 1 bit direction reversal

class MoveHandler {
  public:
    move_t create(dir_t dir, bindex_t rowIndex, bindex_t colIndex, mkind_t moveKind = MKIND_UNDEFINED, dirrev_t dirrev = DIR_NOREV);
    dir_t getDir(move_t move);
    bindex_t getRow(move_t move);
    bindex_t getCol(move_t move);
    mkind_t getKind(move_t move);
    dirrev_t getDirRev(move_t move);
    void print(move_t move);
};

class GameStateHandler {
  public:
    GameStateHandler(len_t initLen); // initLen is number of tiles per side
    ~GameStateHandler();
    len_t len;
    MoveHandler* moveHandler;
    tile_t getTile(state_t state, bindex_t row, bindex_t col);
    state_t setTile(state_t state, bindex_t row, bindex_t col, tile_t tile);
    std::vector<move_t> allMoves(state_t state);
    std::vector<state_t> allPlusParents(state_t state); // reverse (numA, numB) -- -1 --> (numB-1, numA)
    std::vector<state_t> allZeroParents(state_t state); // reverse (numA, numB) --  0 --> (numB, numA)
    state_t makeMove(state_t state, move_t move);
    state_t swapPlayers(state_t state);
    bool containsLine(state_t state, tile_t tileType);
    nbit_t getNumX(state_t state);
    nbit_t getNumO(state_t state);
    void print(state_t state);
  private:
    state_t rowMaskFull;
    state_t colMaskFull;
    state_t diag1MaskFull;
    state_t diag2MaskFull;
    std::unordered_map<move_t, std::tuple<state_t, state_t, state_t>> makeMoveCache; // map from move to (mask, newTile, oldTile)
    std::vector<state_t> allParentsAux(state_t state, mkind_t kind);
};
