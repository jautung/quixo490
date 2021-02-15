#include "GameStateHandler.hpp"
#include "GraphicsHandler.hpp"
#include <chrono>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

namespace {
  float tileWidth = 0.25;
  int nullChoice = -2;
  float alphaTile = 1.0;
  float alphaChoice = 0.8;
  float alphaInsert = 0.5;
  int waitCloseMs = 1000;
}

GraphicsHandler::GraphicsHandler(int initScreenRes, GameStateHandler* initGameStateHandler) {
  screenRes = initScreenRes;
  gameStateHandler = initGameStateHandler;

  if (!glfwInit()) {
    glfwErrorCallback(1, "initialization failed");
  }
  glfwSetErrorCallback(glfwErrorCallback);

  window = glfwCreateWindow(screenRes, screenRes, "Quixo", NULL, NULL);
  if (!window) {
    glfwTerminate();
    glfwErrorCallback(1, "create window failed");
    return;
  }

  glfwSetWindowUserPointer(window, this);
  glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
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

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

GraphicsHandler::~GraphicsHandler() {
  if (!window) { // initialization unsuccessful
    return;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(waitCloseMs));
  glfwDestroyWindow(window);
  glfwTerminate();
}

void GraphicsHandler::glfwErrorCallback(int error, const char* description) {
  std::cerr << "error: " << "glfw: " << description << "\n";
}

void GraphicsHandler::drawBoardBase(state_t state, colormode_t colorMode) {
  auto len = gameStateHandler->len;
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_QUADS); // tiles
  for (bindex_t i = 0; i < len; i++) {
    for (bindex_t j = 0; j < len; j++) {
      auto tileType = gameStateHandler->getTile(state, i, j);
      if (!(i == tileChoiceX && j == tileChoiceY)) {
        drawTile(i, j, tileType, alphaTile, colorMode);
      }
    }
  }
  glEnd();

  glBegin(GL_QUADS); // insertion points and chosen tile
  if (tileChoiceX != nullChoice && tileChoiceY != nullChoice) {
    bool validChoice = false;
    auto moves = gameStateHandler->allMoves(state);
    for (auto move : moves) {
      auto row = gameStateHandler->moveHandler->getRow(move);
      auto col = gameStateHandler->moveHandler->getCol(move);
      if (row != tileChoiceX || col != tileChoiceY) {
        continue;
      }
      validChoice = true;
      auto dir = gameStateHandler->moveHandler->getDir(move);
      if (dir == DIR_LEFT) {
        drawTile(tileChoiceX, len, TILE_X, alphaInsert, colorMode);
        if (insertChoiceX == tileChoiceX && insertChoiceY == len) {
          moveChoice = move;
          gettingInputQ = false;
        }
      } else if (dir == DIR_RIGHT) {
        drawTile(tileChoiceX, -1, TILE_X, alphaInsert, colorMode);
        if (insertChoiceX == tileChoiceX && insertChoiceY == -1) {
          moveChoice = move;
          gettingInputQ = false;
        }
      } else if (dir == DIR_DOWN) {
        drawTile(-1, tileChoiceY, TILE_X, alphaInsert, colorMode);
        if (insertChoiceX == -1 && insertChoiceY == tileChoiceY) {
          moveChoice = move;
          gettingInputQ = false;
        }
      } else if (dir == DIR_UP) {
        drawTile(len, tileChoiceY, TILE_X, alphaInsert, colorMode);
        if (insertChoiceX == len && insertChoiceY == tileChoiceY) {
          moveChoice = move;
          gettingInputQ = false;
        }
      }
    }
    auto tileTypeChoice = gameStateHandler->getTile(state, tileChoiceX, tileChoiceY);
    if (validChoice) {
      drawTile(tileChoiceX, tileChoiceY, tileTypeChoice, alphaChoice, colorMode);
    } else {
      drawTile(tileChoiceX, tileChoiceY, tileTypeChoice, alphaTile, colorMode);
    }
  }
  glEnd();

  glBegin(GL_LINES); // borders
  glColor4f(0, 0, 0, 1);
  auto halfLen = 1.0*len/2;
  for (bindex_t i = 0; i <= len; i++) {
    glVertex3f(-halfLen*tileWidth, (i-halfLen)*tileWidth, 0);
    glVertex3f(halfLen*tileWidth, (i-halfLen)*tileWidth, 0);
  }
  for (bindex_t j = 0; j <= len; j++) {
    glVertex3f((j-halfLen)*tileWidth, -halfLen*tileWidth, 0);
    glVertex3f((j-halfLen)*tileWidth, halfLen*tileWidth, 0);
  }
  glEnd();

  glfwSwapBuffers(window);
}

void GraphicsHandler::drawBoard(state_t state, colormode_t colorMode) {
  if (!window) { // initialization unsuccessful
    return;
  }
  gettingInputQ = false;
  tileChoiceX = nullChoice;
  tileChoiceY = nullChoice;
  insertChoiceX = nullChoice;
  insertChoiceY = nullChoice;
  drawBoardBase(state, colorMode);
  glfwPollEvents();
}

move_t GraphicsHandler::drawBoardGetInput(state_t state, colormode_t colorMode) {
  if (!window) { // initialization unsuccessful
    return gameStateHandler->moveHandler->create(DIR_UNDEFINED, 0, 0);
  }
  gettingInputQ = true;
  tileChoiceX = nullChoice;
  tileChoiceY = nullChoice;
  insertChoiceX = nullChoice;
  insertChoiceY = nullChoice;
  while (gettingInputQ) {
    drawBoardBase(state, colorMode);
    glfwPollEvents();
  }
  return moveChoice;
}

void GraphicsHandler::drawTile(bindex_t i, bindex_t j, tile_t tileType, float alpha, colormode_t colorMode) {
  if ((colorMode == COLOR_NORM && tileType == TILE_X) || (colorMode == COLOR_FLIP && tileType == TILE_O)) {
    glColor4f(1, 0, 0, alpha);
  } else if ((colorMode == COLOR_NORM && tileType == TILE_O) || (colorMode == COLOR_FLIP && tileType == TILE_X)) {
    glColor4f(0, 0, 1, alpha);
  } else {
    glColor4f(0.3, 0.3, 0.3, alpha);
  }
  float left, right, top, bottom;
  getTileLimits(i, j, &left, &right, &top, &bottom);
  glVertex3f(left, top, 0);
  glVertex3f(right, top, 0);
  glVertex3f(right, bottom, 0);
  glVertex3f(left, bottom, 0);
}

void GraphicsHandler::getTileLimits(bindex_t i, bindex_t j, float* left, float* right, float* top, float* bottom) {
  auto halfLen = (gameStateHandler->len-1.0)/2;
  *left = (j-halfLen)*tileWidth - tileWidth/2;
  *right = (j-halfLen)*tileWidth + tileWidth/2;
  *top = (halfLen-i)*tileWidth + tileWidth/2;
  *bottom = (halfLen-i)*tileWidth - tileWidth/2;
}

void GraphicsHandler::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    GraphicsHandler* graphics = static_cast<GraphicsHandler*>(glfwGetWindowUserPointer(window));
    graphics->onMouseButtonLeftPress(xPos, yPos);
  }
}

void GraphicsHandler::onMouseButtonLeftPress(double xRawPos, double yRawPos) {
  if (!gettingInputQ) {
    return;
  }
  auto xPos = 2*xRawPos/screenRes - 1;
  auto yPos = 1 - 2*yRawPos/screenRes;

  auto len = gameStateHandler->len;
  for (bindex_t i = -1; i < len+1; i++) {
    for (bindex_t j = -1; j < len+1; j++) {
      float left, right, top, bottom;
      getTileLimits(i, j, &left, &right, &top, &bottom);
      if (xPos >= left && xPos <= right && yPos <= top && yPos >= bottom) {
        if (i >= 0 && i < len && j >= 0 && j < len) { // choose tile
          tileChoiceX = i;
          tileChoiceY = j;
          insertChoiceX = nullChoice;
          insertChoiceY = nullChoice;
        } else { // choose insertion point
          if (tileChoiceX != nullChoice && tileChoiceY != nullChoice) {
            insertChoiceX = i;
            insertChoiceY = j;
          }
        }
      }
    }
  }
}
