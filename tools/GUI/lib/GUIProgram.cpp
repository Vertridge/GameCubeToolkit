#include "GUIProgram.h"

namespace GUI {

GUIProgram::GUIProgram(ProgramOptions options) : mOptions(options) {}

void GUIProgram::Init() {}

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

  mWindow.EndDraw();
}

void GUIProgram::Update() {}

} // namespace GUI
