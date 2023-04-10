#include "Window.h"

#include <iostream>

using namespace GUI;

Window::Window() {
  std::cout << "Creating Window" << std::endl;
  if (!glfwInit()) {
    std::cerr << "Failed to init window" << std::endl;
  }
  mWindow = glfwCreateWindow(640, 480, "GUI", NULL, NULL);
  if (!mWindow) {
    std::cerr << "Failed to create Window" << std::endl;
  }
  glfwMakeContextCurrent(mWindow);
  gladLoadGL();
}

Window::~Window() {
  std::cout << "Destroying Window" << std::endl;
  glfwDestroyWindow(mWindow);
  glfwTerminate();
}
