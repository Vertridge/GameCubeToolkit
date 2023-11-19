#include "Window.h"

#include <iostream>

// vendor
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
  glfwMaximizeWindow(mWindow);

  glfwMakeContextCurrent(mWindow);
  gladLoadGL();

  // Init imgui
  IMGUI_CHECKVERSION();
  const char *glsl_version = "#version 130";
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void Window::BeginDraw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow();
}

void Window::EndDraw() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(mWindow);
  glfwPollEvents();
}

Window::~Window() {
  std::cout << "Destroying Window" << std::endl;
  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

bool Window::ShouldClose() { return glfwWindowShouldClose(mWindow); }
