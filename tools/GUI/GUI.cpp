#include <iostream>

#include "Window.h"

int main() {
  std::cout << "GUI Started" << std::endl;

  GUI::Window window;

  while (!window.ShouldClose()) {
    window.BeginDraw();

    window.EndDraw();
  }

  return 0;
}
