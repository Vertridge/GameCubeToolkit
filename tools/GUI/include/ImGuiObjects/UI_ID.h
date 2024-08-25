#pragma once

#include <cstdint>

#define ID(id)                                                                 \
  static std::uint32_t GetID() { return static_cast<uint32_t>(id); }

enum class UI_ID {
  UNPACKER_UI = 1,
  LOGGER_UI,
  PACKAGE_VIEWER_UI,
  DOL_DISASSEMBLER_UI,
  DOL_VIEWER_UI,
  TXT_VIEWER_UI,
  BNR_VIEWER_UI,
};
