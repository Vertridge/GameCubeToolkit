#pragma once

#include "FileViewers/FileViewerBase.h"
#include "ImGui/UIBase.h"
#include "ImGuiObjects/UI_ID.h"

// stl
#include <memory>

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
  void OnFileSelected(const std::filesystem::path &path);
  FileViewerBase *currentViewer = nullptr;
  std::vector<std::unique_ptr<FileViewerBase>> mFileViewers;
};

} // namespace UIObjects
