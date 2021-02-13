#include "GameState.hpp"
#include "Graphics.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

#define SCREEN_RES_X (800)
#define SCREEN_RES_Y (800)
#define TILE_WIDTH (0.25)
#define CHOICE_NULL (INT_MAX)
#define ALPHA_CHOICE (0.8)
#define ALPHA_INSERTION (0.5)

static void getTileLimits(index_t i, index_t j, float* left, float* right, float* top, float* bottom) {
  *left = (j-2)*TILE_WIDTH - TILE_WIDTH/2;
  *right = (j-2)*TILE_WIDTH + TILE_WIDTH/2;
  *top = (2-i)*TILE_WIDTH + TILE_WIDTH/2;
  *bottom = (2-i)*TILE_WIDTH - TILE_WIDTH/2;
}

void Graphics::glfwErrorCallback(int error, const char* description) {
  std::cerr << "error: " << "glfw: " << description << "\n";
}

void Graphics::glfwMouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods) {
  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    Graphics* graphics = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->onMouseButtonLeftPress(2*xPos/SCREEN_RES_X-1, 1-2*yPos/SCREEN_RES_Y);
  }
}

void Graphics::onMouseButtonLeftPress(double xPos, double yPos) {
  if (!gettingInputQ) {return;}
  for (index_t i = -1; i < 6; i++) {
    for (index_t j = -1; j < 6; j++) {
      float left, right, top, bottom;
      getTileLimits(i, j, &left, &right, &top, &bottom);
      if (xPos >= left && xPos <= right && yPos <= top && yPos >= bottom) {
        if (i >= 0 && i <= 4 && j >= 0 && j <= 4) { // choose tile
          tileChoiceX = i;
          tileChoiceY = j;
          insertChoiceX = CHOICE_NULL;
          insertChoiceY = CHOICE_NULL;
        } else { // choose insertion point
          if (tileChoiceX != CHOICE_NULL && tileChoiceY != CHOICE_NULL) {
            insertChoiceX = i;
            insertChoiceY = j;
          }
        }
      }
    }
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
  glfwSetWindowUserPointer(window, this);
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

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

void Graphics::drawBoardBase(const GameState* gameState, bool flippedColorsQ) {
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_QUADS); // tiles
  for (index_t i = 0; i < 5; i++) {
    for (index_t j = 0; j < 5; j++) {
      auto tileType = gameState->getTile(i, j);
      if (i == tileChoiceX && j == tileChoiceY) {
        continue;
      } else {
        drawTile(i, j, tileType, 1, flippedColorsQ);
      }
    }
  }
  glEnd();

  glBegin(GL_QUADS); // insertion points
  if (tileChoiceX != CHOICE_NULL && tileChoiceY != CHOICE_NULL) {
    bool validChoice = false;
    auto moves = gameState->allMoves();
    for (const auto& move: moves) {
      if (move.dir == DIR_LEFT || move.dir == DIR_RIGHT) {
        if (move.slideIndex != tileChoiceX || move.crossIndex != tileChoiceY) {
          continue;
        }
        validChoice = true;
        if (move.dir == DIR_LEFT) {
          drawTile(tileChoiceX, 5, TILE_X, ALPHA_INSERTION, flippedColorsQ);
          if (insertChoiceX == tileChoiceX && insertChoiceY == 5) {
            dirChoice = move.dir;
          }
        } else {
          drawTile(tileChoiceX, -1, TILE_X, ALPHA_INSERTION, flippedColorsQ);
          if (insertChoiceX == tileChoiceX && insertChoiceY == -1) {
            dirChoice = move.dir;
          }
        }
      } else if (move.dir == DIR_DOWN || move.dir == DIR_UP) {
        if (move.slideIndex != tileChoiceY || move.crossIndex != tileChoiceX) {
          continue;
        }
        validChoice = true;
        if (move.dir == DIR_DOWN) {
          drawTile(-1, tileChoiceY, TILE_X, ALPHA_INSERTION, flippedColorsQ);
          if (insertChoiceX == -1 && insertChoiceY == tileChoiceY) {
            dirChoice = move.dir;
          }
        } else {
          drawTile(5, tileChoiceY, TILE_X, ALPHA_INSERTION, flippedColorsQ);
          if (insertChoiceX == 5 && insertChoiceY == tileChoiceY) {
            dirChoice = move.dir;
          }
        }
      }
    }
    auto tileTypeChoice = gameState->getTile(tileChoiceX, tileChoiceY);
    if (validChoice) {
      drawTile(tileChoiceX, tileChoiceY, tileTypeChoice, ALPHA_CHOICE, flippedColorsQ);
    } else {
      drawTile(tileChoiceX, tileChoiceY, tileTypeChoice, 1, flippedColorsQ);
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
}

void Graphics::drawBoard(const GameState* gameState) {
  if (!window) {return;} // initialization unsuccessful
  gettingInputQ = false;
  tileChoiceX = CHOICE_NULL;
  tileChoiceY = CHOICE_NULL;
  drawBoardBase(gameState);
  glfwPollEvents();
}

Move Graphics::drawBoardGetInput(const GameState* gameState, bool flippedColorsQ) {
  if (!window) {return Move(DIR_LEFT, 0, 0);} // initialization unsuccessful
  gettingInputQ = true;
  tileChoiceX = CHOICE_NULL;
  tileChoiceY = CHOICE_NULL;
  insertChoiceX = CHOICE_NULL;
  insertChoiceY = CHOICE_NULL;
  dirChoice = DIR_UNDEFINED;
  while (dirChoice == DIR_UNDEFINED) {
    drawBoardBase(gameState, flippedColorsQ);
    glfwPollEvents();
  }
  if (dirChoice == DIR_LEFT || dirChoice == DIR_RIGHT) {
    return Move(dirChoice, tileChoiceX, tileChoiceY);
  } else if (dirChoice == DIR_DOWN || dirChoice == DIR_UP) {
    return Move(dirChoice, tileChoiceY, tileChoiceX);
  } else {
    return Move(DIR_LEFT, 0, 0); // dummy
  }
}

void Graphics::terminate() {
  if (!window) {return;} // initialization unsuccessful
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Graphics::drawTile(index_t i, index_t j, tile_t tileType, float alpha, bool flippedColorsQ) {
  if ((!flippedColorsQ && tileType == TILE_X) || (flippedColorsQ && tileType == TILE_O)) {
    glColor4f(1, 0, 0, alpha);
  } else if ((!flippedColorsQ && tileType == TILE_O) || (flippedColorsQ && tileType == TILE_X)) {
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
