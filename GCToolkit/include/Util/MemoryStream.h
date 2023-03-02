#pragma once

#include <istream>

namespace util {

class MemoryBuffer : public std::basic_streambuf<char> {
public:
  MemoryBuffer(const uint8_t *p, size_t l);
};

class MemoryStream : public std::istream {
public:
  MemoryStream(const uint8_t *p, size_t l);

private:
  MemoryBuffer mBuffer;
};

} // namespace util
