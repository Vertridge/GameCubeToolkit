#pragma once

#include <Platform/FileSelector.h>

#include <filesystem>
#include <string>

namespace Support {

class DiskSingleton {
public:
  DiskSingleton(DiskSingleton const &) = delete;
  void operator=(DiskSingleton const &) = delete;
  static DiskSingleton &GetSingleton() {
    static DiskSingleton instance;
    return instance;
  }

  std::filesystem::path ProjectDir;
  std::filesystem::path Iso;
  std::filesystem::path Disassembly;
  std::filesystem::path Dump;

private:
  DiskSingleton() = default;
};

inline void ChooseProjectDir() {
  DiskSingleton::GetSingleton().ProjectDir =
      Platform::FileSelector::SelectDirectory("Select project directory");
}

} // namespace Support
