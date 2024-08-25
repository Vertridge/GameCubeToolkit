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
  // std::unique_ptr<UIBase> &GetUi(int id);
  Error AddChildToUi(int parentId, int childId, std::unique_ptr<UIBase> ui);
  void Draw();

private:
  struct ChildInfo {
    ChildInfo(int parentId, std::unique_ptr<UIBase> child)
        : parentId(parentId), childUi(std::move(child)) {}
    int parentId;
    std::unique_ptr<UIBase> childUi;
  };

  std::unordered_map<int, std::unique_ptr<UIBase>> mUi;
  std::unordered_map<int, ChildInfo> mChildren;
  bool mInit = false;
};

} // namespace GUILib
