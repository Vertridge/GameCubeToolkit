#pragma once

#include "ImGui/UIBase.h"
#include "ImGuiObjects/UI_ID.h"

using namespace GUILib;

namespace UIObjects {

class PackageViewerGui : public UIBase {
public:
  ID(UI_ID::PACKAGE_VIEWER_UI);

  void OnInit() override;
  void OnBeginDraw() override;
  void OnEndDraw() override;

private:
  void DrawFileSystem();
};

} // namespace UIObjects
