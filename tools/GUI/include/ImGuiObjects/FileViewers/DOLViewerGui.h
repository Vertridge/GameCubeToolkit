#pragma once

#include "FileViewerBase.h"
#include "ImGuiObjects/UI_ID.h"

using namespace GUILib;

namespace UIObjects {

class DOLViewerGui : public FileViewerBase {
public:
  ID(UI_ID::DOL_VIEWER_UI);

  DOLViewerGui();

  void OnInit() override;
  void OnBeginDraw() override;
  void OnEndDraw() override;
  void OnSetFile() override;

private:
  std::string dolHeader;
};

} // namespace UIObjects
