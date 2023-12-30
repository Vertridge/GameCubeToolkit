#pragma once

#define ID(id)                                                                 \
  static int GetID() { return id; };

enum UI_ID { UNPACKER_UI = 1 };
