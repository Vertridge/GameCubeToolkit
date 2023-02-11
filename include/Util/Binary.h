#pragma once

#include <cstdint>

namespace util {

/// Swaps the endiness of 32bit int.
void SwapBinary(std::uint32_t &value) {
  std::uint32_t tmp = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF);
  value = (tmp << 16) | (tmp >> 16);
}

} // namespace util
