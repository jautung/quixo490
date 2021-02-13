#pragma once
#include "GameState.hpp"
#include <GLFW/glfw3.h>

class Graphics {
  public:
    Graphics();
    void drawBoard(const GameState* gameState);
    Move drawBoardGetInput(const GameState* gameState);
    void terminate();
  private:
    GLFWwindow* window;
    static void glfwErrorCallback(int error, const char* description);
    static void glfwMouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
    void drawBoardBase(const GameState* gameState);
    void drawTile(index_t i, index_t j, tile_t tileType, float alpha = 1);
    void onMouseButtonLeftPress(double xpos, double ypos);
    bool gettingInputQ;
    index_t tileChoiceX;
    index_t tileChoiceY;
    index_t insertChoiceX;
    index_t insertChoiceY;
    dir_t dirChoice;
};
