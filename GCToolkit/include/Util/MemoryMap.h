#pragma once

#include <string>

namespace util {

struct MmapMem {
  void *Addr = nullptr;
  std::size_t Len = 0;
};

MmapMem MmapFile(const std::string &file);

void MunMap(MmapMem);

} // namespace util
