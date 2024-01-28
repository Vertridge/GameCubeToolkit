#include "Window.h"

#include <iostream>

// Util
#include <Logger/Logger.h>

// vendor
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace GUI;

Window::Window() {
  LOG_TRACE("Creating Window");

  if (!glfwInit()) {
    LOG_ERROR("Failed to init window");
    return;
  }

  mWindow = glfwCreateWindow(640, 480, "GUI", NULL, NULL);
  if (!mWindow) {
    LOG_ERROR("Failed to create Window");
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
  LOG_TRACE("Destroying Window");
  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

bool Window::ShouldClose() { return glfwWindowShouldClose(mWindow); }
