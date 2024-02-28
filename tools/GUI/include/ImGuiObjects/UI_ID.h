#pragma once

#define ID(id)                                                                 \
  static int GetID() { return static_cast<int>(id); }

enum class UI_ID { UNPACKER_UI = 1, LOGGER_UI = 2, PACKAGE_VIEWER_UI = 3 };
