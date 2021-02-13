#pragma once
#include "GameState.hpp"
#include <GLFW/glfw3.h>

class Graphics {
  public:
    Graphics();
    void drawBoard(const GameState* gameState);
    void drawBoardWaitInput(const GameState* gameState);
    void terminate();
  private:
    GLFWwindow* window;
    void drawTile(index_t i, index_t j, tile_t tileType);
};
