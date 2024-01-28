#pragma once

#include "UIBase.h"

#include "Error.h"
using namespace Util;

#include <memory>
#include <unordered_map>

namespace GUILib {

class UIManager {
public:
  void Init();
  Error AddUi(int id, std::unique_ptr<UIBase> ui);
  Error RemoveUi(int id);
  void Draw();

private:
  std::unordered_map<int, std::unique_ptr<UIBase>> mUi;
};

} // namespace GUILib
