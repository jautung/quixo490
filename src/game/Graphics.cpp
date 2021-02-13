#include <GLFW/glfw3.h>
#include <iostream>

#define SCREEN_RES_X (800)
#define SCREEN_RES_Y (800)

void glfwErrorCallback(int error, const char* description) {
  std::cerr << "error: " << "glfw: " << description << "\n";
}

void glfwKeyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS && (key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void glfwMouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods) {
  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << xpos << " " << ypos << "\n";
  }
}

void render() {
  glBegin(GL_QUADS);
  glColor4f(1, 0, 0, 1);
  glVertex3f(-0.5, -0.5, 0);
  glVertex3f(-0.5, 0.5, 0);
  glVertex3f(0.5, 0.5, 0);
  glVertex3f(0.5, -0.5, 0);
  glEnd();
}

void testGraphics() {
  if (!glfwInit()) {
    glfwErrorCallback(1, "initialization failed");
  }
  glfwSetErrorCallback(glfwErrorCallback);

  GLFWwindow* window = glfwCreateWindow(SCREEN_RES_X, SCREEN_RES_Y, "Quixo", NULL, NULL);
  if (!window) {
    glfwTerminate();
    glfwErrorCallback(1, "create window failed");
    return;
  }
  glfwSetKeyCallback(window, glfwKeyCallbacks);
  glfwSetMouseButtonCallback(window, glfwMouseButtonCallbacks);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  auto ratio = 1.0 * width / height;
  glOrtho(-ratio, ratio, -1, 1, 1, -1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  while (!glfwWindowShouldClose(window)) {
    render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
