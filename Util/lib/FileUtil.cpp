#include "FileUtil.h"
#include <sys/stat.h>

namespace util {

std::size_t GetFileSize(const std::string &file) {
  struct stat stat_buf;
  int rc = stat(file.c_str(), &stat_buf);
  return rc == 0 ? stat_buf.st_size : 0;
}

} // namespace util
