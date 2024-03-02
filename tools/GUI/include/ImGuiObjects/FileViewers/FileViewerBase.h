#pragma once

#include "ImGui/UIBase.h"

// stl
#include <filesystem>

using namespace GUILib;

namespace UIObjects {

class FileViewerBase : public UIBase {
public:
  FileViewerBase() { SetEnabled(false); }
  bool SetFile(const std::filesystem::path &file);
  virtual void OnSetFile() = 0;

protected:
  std::string mExtension;
  std::filesystem::path mFile;
};

} // namespace UIObjects
