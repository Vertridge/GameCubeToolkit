#pragma once

#include <memory>
#include <vector>

class UIBase {
public:
  UIBase() = default;
  virtual ~UIBase() = default;

  void Init();
  virtual void OnInit() = 0;
  void BeginDraw();
  virtual void OnBeginDraw() = 0;
  void EndDraw();
  virtual void OnEndDraw() = 0;

  void SetParent(UIBase *parent);

  void AddChild(UIBase *child);

private:
  std::vector<UIBase *> mChildren;
  UIBase *mParent = nullptr;
};
