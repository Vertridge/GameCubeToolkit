#pragma once

#include "ImGui/UIBase.h"
#include "ImGuiObjects/UI_ID.h"

using namespace GUILib;

namespace UIObjects {

class UnpackerGui : public UIBase {
public:
  ID(UI_ID::UNPACKER_UI)

  void OnInit() override;
  void OnBeginDraw() override;
  void OnEndDraw() override;
};

} // namespace UIObjects
