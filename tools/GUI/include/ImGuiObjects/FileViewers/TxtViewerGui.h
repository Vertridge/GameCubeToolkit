#pragma once

#include "FileViewerBase.h"
#include "ImGuiObjects/UI_ID.h"

namespace UIObjects {

class TxtViewerGui : public FileViewerBase {
public:
  ID(UI_ID::TXT_VIEWER_UI);

  TxtViewerGui();

  void OnInit() override;
  void OnBeginDraw() override;
  void OnEndDraw() override;
  void OnSetFile() override;

private:
  std::string mTxtContent;
};

} // namespace UIObjects
