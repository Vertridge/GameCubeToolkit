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

  static GUIProgram *GetInstance() { return mInstance; }

  void Init();

  void Start();
  void Run();

  void Render();
  void Update();

  void End();

  GUILib::UIManager &GetUiManager() { return mUiManager; }

private:
  void InitUI();

  bool mExit = false;

  Window mWindow;
  ProgramOptions mOptions;
  GUILib::UIManager mUiManager;

  static GUIProgram *mInstance;
};

} // namespace GUI
