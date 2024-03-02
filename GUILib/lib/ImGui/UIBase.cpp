#include "ImGui/UIBase.h"

#include <cassert>

namespace GUILib {

void UIBase::Init() {
  OnInit();
  for (auto &child : mChildren) {
    child->Init();
  }
}

void UIBase::BeginDraw() {
  if (!mEnabled) {
    return;
  }
  OnBeginDraw();
  for (auto &child : mChildren) {
    child->BeginDraw();
  }
}

void UIBase::EndDraw() {
  if (!mEnabled) {
    return;
  }
  for (auto &child : mChildren) {
    child->EndDraw();
  }
  OnEndDraw();
}

void UIBase::SetParent(UIBase *parent) {
  assert(mParent == nullptr && "Every child can only have 1 parent.");
  mParent = parent;
}

void UIBase::AddChild(UIBase *child) {
  child->SetParent(this);
  mChildren.push_back(child);
}

} // namespace GUILib
