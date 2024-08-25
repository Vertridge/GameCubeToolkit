#include "GUIProgram.h"

// GCToolkit
#include <Support/DiskSingleton.h>

// UI
#include <ImGuiObjects/LoggerGui.h>
#include <ImGuiObjects/PackageViewerGui.h>
#include <ImGuiObjects/UnpackerGui.h>

// Util
#include <Logger/Logger.h>

// stl
#include <iostream>

namespace GUI {

GUIProgram *GUIProgram::mInstance = nullptr;

GUIProgram::GUIProgram(ProgramOptions options) : mOptions(options) {
  mInstance = this;
}

void GUIProgram::Init() {
  auto &disk = Support::DiskSingleton::GetSingleton();
  if (!mOptions.projectDir.empty()) {
    disk.ProjectDir = mOptions.projectDir;
  }
  disk.Dump = mOptions.dump;
  disk.Disassembly = mOptions.dis;
  disk.Iso = mOptions.inputISO;

  InitUI();
}

void GUIProgram::InitUI() {
  LOG_TRACE("Init UI");
  mUiManager.AddUi(UIObjects::UnpackerGui::GetID(),
                   std::make_unique<UIObjects::UnpackerGui>());

  mUiManager.AddUi(UIObjects::LoggerGui::GetID(),
                   std::make_unique<UIObjects::LoggerGui>());

  mUiManager.AddChildToUi(UIObjects::UnpackerGui::GetID(),
                          UIObjects::PackageViewerGui::GetID(),
                          std::make_unique<UIObjects::PackageViewerGui>());

  mUiManager.Init();
}

void GUIProgram::Start() {

  while (!mExit && !mWindow.ShouldClose()) {
    Run();
  }

  End();
}

void GUIProgram::Run() {
  Update();
  Render();
}

void GUIProgram::End() {}

void GUIProgram::Render() {
  mWindow.BeginDraw();

  mUiManager.Draw();

  mWindow.EndDraw();
}

void GUIProgram::Update() {}

} // namespace GUI
