#include "Util/MemoryStream.h"

using namespace util;

MemoryBuffer::MemoryBuffer(const uint8_t *p, size_t l) {
  setg((char *)p, (char *)p, (char *)p + l);
}

MemoryStream::MemoryStream(const uint8_t *p, size_t l)
    : std::istream(&mBuffer), mBuffer(p, l) {
  rdbuf(&mBuffer);
}
