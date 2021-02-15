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

typedef uint16_t move_t;  // from LSB to MSB: 4 bits direction, 6 bits row index, 6 bits column index

class MoveHandler {
  public:
    move_t create(dir_t dir, bindex_t rowIndex, bindex_t colIndex);
    dir_t getDir(move_t move);
    bindex_t getRow(move_t move);
    bindex_t getCol(move_t move);
    void print(move_t move);
};

class GameStateHandler {
  public:
    GameStateHandler(len_t initLen); // initLen is number of tiles per side
    ~GameStateHandler();
    len_t len;
    MoveHandler* moveHandler;
    tile_t getTile(state_t state, bindex_t row, bindex_t col);
    std::vector<move_t> allMoves(state_t state);
    state_t makeMove(state_t state, move_t move);
    state_t swapPlayers(state_t state);
    bool containsLine(state_t state, tile_t tileType);
    void print(state_t state);
  private:
    state_t rowMaskFull;
    state_t colMaskFull;
    state_t diag1MaskFull;
    state_t diag2MaskFull;
    std::unordered_map<move_t, std::tuple<state_t, state_t>> makeMoveCache; // map from move to (mask, newTile)
};
