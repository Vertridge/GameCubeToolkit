#include "ImGuiObjects/FileViewers/FileViewerBase.h"

// Util
#include <String.h>

// stl
#include <algorithm> // std::equal
#include <cctype>

using namespace GUILib;

namespace UIObjects {

bool FileViewerBase::SetFile(const std::filesystem::path &file) {
  if (!util::CaseInsensitiveCompare(file.extension(), mExtension)) {
    return false;
  }
  mFile = file;
  OnSetFile();
  return true;
}

} // namespace UIObjects
