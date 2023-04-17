#include "ImGui/UIBase.h"

#include <cassert>

void UIBase::Init() {
  OnInit();
  for (auto &child : mChildren) {
    child->Init();
  }
}

void UIBase::BeginDraw() {
  OnBeginDraw();
  for (auto &child : mChildren) {
    child->BeginDraw();
  }
}

void UIBase::EndDraw() {
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
