#pragma once

#include <memory>
#include <vector>

namespace GUILib {

class UIBase {
public:
  UIBase() = default;
  virtual ~UIBase() = default;

  static int GetID() { return -1; }

  void Init();
  virtual void OnInit() = 0;
  void BeginDraw();
  virtual void OnBeginDraw() = 0;
  void EndDraw();
  virtual void OnEndDraw() = 0;

  void SetParent(UIBase *parent);

  void AddChild(UIBase *child);

  void SetEnabled(bool b) { mEnabled = b; }
  bool IsEnabled() { return mEnabled; }

private:
  bool mEnabled = true;
  std::vector<UIBase *> mChildren;
  UIBase *mParent = nullptr;
};

} // namespace GUILib
