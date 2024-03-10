#pragma once

#include "FileViewerBase.h"
#include "ImGuiObjects/UI_ID.h"

using namespace GUILib;

namespace UIObjects {

class BNRViewerGui : public FileViewerBase {
public:
  ID(UI_ID::BNR_VIEWER_UI);

  BNRViewerGui();

  void OnInit() override;
  void OnBeginDraw() override;
  void OnEndDraw() override;
  void OnSetFile() override;

private:
  std::string mBnrHeader;
};

} // namespace UIObjects
