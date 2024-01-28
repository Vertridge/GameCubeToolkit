#pragma once

#include "ImGui/UIBase.h"
#include "ImGuiObjects/UI_ID.h"

// Util
#include <Logger/LoggerBase.h>

// vendor
#include <imgui.h>

// stl
#include <mutex>

using namespace GUILib;
using namespace util;

namespace UIObjects {

class LoggerGui : public UIBase, public LoggerBase {
public:
  ID(UI_ID::LOGGER_UI);

  void OnInit() override;
  void OnBeginDraw() override;
  void OnEndDraw() override;

  void Log(Verbosity verbosity, const std::string &str) override;

  void Clear();

  ~LoggerGui();

private:
  void AddToBuffer(const std::string &str);
  ImGuiTextBuffer mBuf;
  ImGuiTextFilter mFilter;
  // Index to lines offset. We maintain this with AddLog() calls.
  ImVector<int> mLineOffsets;
  // Keep scrolling if already at the bottom.
  bool mAutoScroll;

  std::mutex mLock;
};

} // namespace UIObjects
