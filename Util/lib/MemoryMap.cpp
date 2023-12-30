#include "MemoryMap.h"
#include "FileUtil.h"

#include <fcntl.h>
#include <sys/mman.h>

namespace util {

MmapMem MmapFile(const std::string &file) {
  std::size_t len = GetFileSize(file);
  int fd = open(file.c_str(), O_RDWR);
  void *addr = mmap(nullptr, len, PROT_READ, MAP_SHARED, fd, 0);

  return {addr, len};
}

void MunMap(MmapMem mem) {
  if (mem.Addr == nullptr) {
    return;
  }
  munmap(mem.Addr, mem.Len);
}

} // namespace util
