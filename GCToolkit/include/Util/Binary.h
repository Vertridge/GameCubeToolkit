#pragma once

#include <cassert>
#include <cstdint>
#include <string>

namespace util {

enum class Endian { BigEndian, LittleEndian };

std::string ToString(Endian endian) {
  switch (endian) {
  case Endian::BigEndian:
    return "Big Endian";
  case Endian::LittleEndian:
    return "Little Endian";
  default:
    assert(false);
    return "Unknown";
  }
}

Endian GetHostEndiannes() {
  int i = 1;
  return (*((char *)&i) == 1) ? Endian::LittleEndian : Endian::BigEndian;
}

/// Swaps the endiness of 32bit int.
void SwapBinary(std::uint32_t &value) {
  std::uint32_t tmp = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF);
  value = (tmp << 16) | (tmp >> 16);
}

} // namespace util
