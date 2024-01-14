#pragma once

#include <cinttypes>

#include <ostream>

namespace util {

struct int24 {
  std::uint8_t bytes[3];

  int24 &operator=(const std::int32_t &value) {
    bytes[0] = (value & 0x000000ff);
    bytes[1] = (value & 0x0000ff00) >> 8;
    bytes[2] = (value & 0x00ff0000) >> 16;
    return *this;
  }

  operator std::int32_t() const {
    std::int32_t result = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16);
    if (result & 0x800000) {
      return result |= ~0xffffff;
    }
    return result;
  }
};

struct uint24 {
  std::uint8_t bytes[3];
  uint24 &operator=(const std::uint32_t &value) {
    bytes[0] = (value & 0x000000ff);
    bytes[1] = (value & 0x0000ff00) >> 8;
    bytes[2] = (value & 0x00ff0000) >> 16;
    return *this;
  }

  operator std::uint32_t() const {
    std::uint32_t result = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16);
    return result;
  }

  // friend std::ostream &operator<<(std::ostream &os, const uint24 val) {
  //   std::uint32_t tmp = val;
  //   os << tmp;
  //   return os;
  // }
};

} // namespace util
