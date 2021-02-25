#pragma once
#include "GameStateHandler.hpp"
#include <cstdint>

#ifdef __APPLE__
#include <GLFW/glfw3.h>
#endif // __APPLE__

enum colormode_t : uint8_t {
  COLOR_NORM,
  COLOR_FLIP
};

class GraphicsHandler {
  public:
    GraphicsHandler(GameStateHandler* initGameStateHandler, int initScreenRes);
    ~GraphicsHandler();
    void drawBoard(state_t state, colormode_t colorMode = COLOR_NORM);
    move_t drawBoardGetInput(state_t state, colormode_t colorMode = COLOR_NORM);
  private:
    GameStateHandler* gameStateHandler;

#ifdef __APPLE__

    int screenRes;
    static void glfwErrorCallback(int error, const char* description);
    GLFWwindow* window;
    void drawBoardBase(state_t state, colormode_t colorMode = COLOR_NORM);
    void drawTile(bindex_t i, bindex_t j, tile_t tileType, float alpha = 1, colormode_t colorMode = COLOR_NORM);
    void getTileLimits(bindex_t i, bindex_t j, float* left, float* right, float* top, float* bottom);
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void onMouseButtonLeftPress(double xRawPos, double yRawPos);
    bool gettingInputQ;
    bindex_t tileChoiceX;
    bindex_t tileChoiceY;
    bindex_t insertChoiceX;
    bindex_t insertChoiceY;
    move_t moveChoice;

#endif // __APPLE__

};
