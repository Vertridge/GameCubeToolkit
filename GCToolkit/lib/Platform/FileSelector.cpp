#include "Platform/FileSelector.h"

namespace Platform::FileSelector {

std::string SelectFile() {
  char filename[1024];
  FILE *f = popen("zenity --file-selection", "r");
  fgets(filename, 1024, f);
  std::string file = filename;
  // Remove ending newline
  if (!file.empty() && file[file.length() - 1] == '\n') {
    file.erase(file.length() - 1);
  }
  return file;
}
} // namespace Platform::FileSelector
