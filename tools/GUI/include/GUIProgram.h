#pragma once

#include "ImGui/UIManager.h"
#include "Window.h"

// stl
#include <string>

namespace GUI {

struct ProgramOptions {
  std::string inputISO;
  std::string projectDir;
  std::string dis;
  std::string dump;
};

class GUIProgram {

public:
  GUIProgram(ProgramOptions options);
  void Init();

  void Start();
  void Run();

  void Render();
  void Update();

  void End();

private:
  void InitUI();

  bool mExit = false;

  Window mWindow;
  ProgramOptions mOptions;
  GUILib::UIManager mUiManager;
};

} // namespace GUI
