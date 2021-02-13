#pragma once
#include <unordered_set>

typedef int index_t;
typedef unsigned long state_t;

enum tile_t {
  TILE_X,
  TILE_O,
  TILE_EMPTY
};

enum dir_t {
  DIR_LEFT,
  DIR_RIGHT,
  DIR_DOWN,
  DIR_UP,
  DIR_UNDEFINED
};

class Move {
  public:
    Move(dir_t initDir, index_t initSlideIndex, index_t initCrossIndex);
    dir_t dir;
    index_t slideIndex; // either row or column index that slides (0 through 4 inclusive)
    index_t crossIndex; // either row or column index where tile is picked up (0 through 4 inclusive)

    bool operator==(const Move& otherMove) const {
      return this->dir == otherMove.dir && this->slideIndex == otherMove.slideIndex && this->crossIndex == otherMove.crossIndex;
    }
    struct Hash {
      std::size_t operator()(const Move& move) const {
        return move.dir << 6 | move.slideIndex << 3 | move.crossIndex;
      }
    };
};
std::ostream &operator<<(std::ostream &os, const Move& move);

class GameState {
  public:
    GameState(state_t initState = 0b0);
    tile_t getTile(index_t row, index_t col) const;
    std::unordered_set<Move, Move::Hash> allMoves() const;
    void makeMove(Move move);
    void swapPlayers();
    bool containsLine(tile_t tileType) const;
  private:
    state_t state: 64;
};
std::ostream &operator<<(std::ostream &os, const GameState& gameState);
