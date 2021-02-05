#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class GameState {
  public:
    GameState(int init);
    ~GameState();
    int play();
  private:
    int board;
    int player;
};

#endif
