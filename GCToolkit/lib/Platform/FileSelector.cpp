#include "Platform/FileSelector.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace Platform::FileSelector {

namespace {
std::string MergeCommand(const std::string &command,
                         const std::vector<std::string> &args) {
  std::ostringstream implode;
  implode << command << " ";
  std::for_each(args.begin(), args.end(),
                [&implode](auto str) { implode << std::quoted(str) << " "; });
  return implode.str();
}

std::string Zenity(const std::vector<std::string> &args) {
  char filename[1024];
  std::string command = MergeCommand("zenity", args);
  FILE *f = popen(command.c_str(), "r");
  fgets(filename, 1024, f);
  std::string file = filename;
  // Remove ending newline
  if (!file.empty() && file[file.length() - 1] == '\n') {
    file.erase(file.length() - 1);
  }
  return file;
}

} // namespace

std::string SelectFile(const std::string &title) {
  return Zenity({"--file-selection", "--title", title});
}

std::string SelectDirectory(const std::string &title) {
  return Zenity({"--file-selection", "--title", title, "--directory"});
}

} // namespace Platform::FileSelector
