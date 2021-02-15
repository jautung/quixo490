#pragma once
#include "GameStateHandler.hpp"
#include <GLFW/glfw3.h>

enum colormode_t : uint8_t {
  COLOR_NORM,
  COLOR_FLIP
};

class GraphicsHandler {
  public:
    GraphicsHandler(int initScreenResX, int initScreenResY, GameStateHandler* initGameStateHandler);
    ~GraphicsHandler();
    void drawBoard(state_t state, colormode_t colorMode = COLOR_NORM);
    move_t drawBoardGetInput(state_t state, colormode_t colorMode = COLOR_NORM);
  private:
    int screenResX;
    int screenResY;
    GameStateHandler* gameStateHandler;
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
    dir_t dirChoice;
};
