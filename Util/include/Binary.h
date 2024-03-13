#pragma once

#include "int24.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
namespace util {

enum class Endian { BigEndian, LittleEndian };

inline std::string ToString(Endian endian) {
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

inline Endian GetHostEndiannes() {
  int i = 1;
  return (*((char *)&i) == 1) ? Endian::LittleEndian : Endian::BigEndian;
}

/// Swaps the endiness of 32bit int.
inline void SwapBinary(std::uint32_t &value) {
  std::uint32_t tmp = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF);
  value = (tmp << 16) | (tmp >> 16);
}

inline std::uint32_t GetSwapBinary(std::uint32_t value) {
  SwapBinary(value);
  return value;
}

/// Swaps the endiness of 24bit int.
inline void SwapBinary(util::uint24 &value) {
  std::uint32_t tmp = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF);
  value = (tmp << 16) | (tmp >> 16);
  value = value >> 8;
}

inline util::uint24 GetSwapBinary(util::uint24 value) {
  SwapBinary(value);
  return value;
}

/// Swaps the endiness of 16bit int.
inline void SwapBinary(std::uint16_t &value) {
  value = (value << 8) | (value >> 8);
  // std::uint16_t tmp = ((value << 8) & 0xFF00) | ((value >> 8) & 0x00FF);
  // value = (tmp << 16) | (tmp >> 16);
}

inline std::uint16_t GetSwapBinary(std::uint16_t value) {
  SwapBinary(value);
  return value;
}

} // namespace util
