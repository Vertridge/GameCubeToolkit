#pragma once

#define ID(id)                                                                 \
  static int GetID() { return static_cast<int>(id); }

enum class UI_ID {
  UNPACKER_UI = 1,
  LOGGER_UI,
  PACKAGE_VIEWER_UI,
  DOL_VIEWER_UI,
  TXT_VIEWER_UI,
};
