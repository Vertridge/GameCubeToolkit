#pragma once

#include "Window.h"

// stl
#include <string>

namespace GUI {

struct ProgramOptions {
  std::string inputISO;
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
  bool mExit = false;

  Window mWindow;
  ProgramOptions mOptions;
};

} // namespace GUI
