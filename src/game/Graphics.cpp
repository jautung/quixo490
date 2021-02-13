#include "GameState.hpp"
#include "Graphics.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

#define SCREEN_RES_X (800)
#define SCREEN_RES_Y (800)
#define TILE_WIDTH (0.25)

static void glfwErrorCallback(int error, const char* description) {
  std::cerr << "error: " << "glfw: " << description << "\n";
}

static void glfwMouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods) {
  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cerr << xpos << " " << ypos << "\n";
  }
}

Graphics::Graphics() {
  if (!glfwInit()) {
    glfwErrorCallback(1, "initialization failed");
  }
  glfwSetErrorCallback(glfwErrorCallback);

  window = glfwCreateWindow(SCREEN_RES_X, SCREEN_RES_Y, "Quixo", NULL, NULL);
  if (!window) {
    glfwTerminate();
    glfwErrorCallback(1, "create window failed");
    return;
  }
  glfwSetMouseButtonCallback(window, glfwMouseButtonCallbacks);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  auto ratio = 1.0 * width / height;
  glOrtho(-ratio, ratio, -1, 1, 1, -1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Graphics::drawBoard(const GameState* gameState) {
  if (!window) {return;} // initialization unsuccessful
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_QUADS); // tiles
  for (index_t i = 0; i < 5; i++) {
    for (index_t j = 0; j < 5; j++) {
      auto tileType = gameState->getTile(i, j);
      drawTile(i, j, tileType);
    }
  }
  glEnd();

  glBegin(GL_LINES); // borders
  glLineWidth(10);
  glColor4f(0, 0, 0, 1);
  for (index_t i = 0; i < 6; i++) {
    glVertex3f(-2.5*TILE_WIDTH, (2.5-i)*TILE_WIDTH, 0);
    glVertex3f(2.5*TILE_WIDTH, (2.5-i)*TILE_WIDTH, 0);
  }
  for (index_t j = 0; j < 6; j++) {
    glVertex3f((j-2.5)*TILE_WIDTH, -2.5*TILE_WIDTH, 0);
    glVertex3f((j-2.5)*TILE_WIDTH, 2.5*TILE_WIDTH, 0);
  }
  glEnd();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Graphics::drawBoardWaitInput(const GameState* gameState) {
  if (!window) {return;} // initialization unsuccessful
}

void Graphics::terminate() {
  if (!window) {return;} // initialization unsuccessful
  glfwDestroyWindow(window);
  glfwTerminate();
}

static void getTileLimits(index_t i, index_t j, float* left, float* right, float* top, float* bottom) {
  *left = (j-2)*TILE_WIDTH - TILE_WIDTH/2;
  *right = (j-2)*TILE_WIDTH + TILE_WIDTH/2;
  *top = (2-i)*TILE_WIDTH + TILE_WIDTH/2;
  *bottom = (2-i)*TILE_WIDTH - TILE_WIDTH/2;
}

void Graphics::drawTile(index_t i, index_t j, tile_t tileType) {
  if (tileType == TILE_X) {
    glColor4f(1, 0, 0, 1);
  } else if (tileType == TILE_O) {
    glColor4f(0, 0, 1, 1);
  } else {
    glColor4f(0.3, 0.3, 0.3, 1);
  }
  float left, right, top, bottom;
  getTileLimits(i, j, &left, &right, &top, &bottom);
  glVertex3f(left, top, 0);
  glVertex3f(right, top, 0);
  glVertex3f(right, bottom, 0);
  glVertex3f(left, bottom, 0);
}
