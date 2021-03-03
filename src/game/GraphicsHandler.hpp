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

enum inputmode_t : uint8_t {
  INPUT_NONE,
  INPUT_GET_MOVE,
  INPUT_GET_STATE
};

class GraphicsHandler {
  public:
    GraphicsHandler(GameStateHandler* initGameStateHandler, int initScreenRes);
    ~GraphicsHandler();
    void drawBoard(state_t state, colormode_t colorMode = COLOR_NORM);
    move_t drawBoardGetMove(state_t state, colormode_t colorMode = COLOR_NORM);
    state_t drawBaseBoardGetState(colormode_t colorMode = COLOR_NORM);
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
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onEnterKeyPress();
    inputmode_t gettingInputType;
    bindex_t getMoveTileChoiceX;
    bindex_t getMoveTileChoiceY;
    bindex_t getMoveInsertChoiceX;
    bindex_t getMoveInsertChoiceY;
    move_t getMoveChoice;
    state_t getStateState;

#endif // __APPLE__

};
