#include "ImGui/UIManager.h"

namespace GUILib {

void UIManager::Init() {
  for (auto &ui : mUi) {
    ui.second->Init();
  }
  mInit = true;
}

Error UIManager::AddUi(int id, std::unique_ptr<UIBase> ui) {
  if (mUi.find(id) != mUi.end()) {
    return MakeError("Key already exists in map");
  }

  // If the UI is allready initialised, init the UI element when it is added.
  if (mInit) {
    ui->Init();
  }
  mUi.emplace(id, std::move(ui));
  return Error::Success();
}

Error UIManager::RemoveUi(int id) {
  auto ui = mUi.find(id);
  if (ui == mUi.end()) {
    return MakeError("Key does already exists in map");
  }
  mUi.erase(ui);
  return Error::Success();
}

void UIManager::Draw() {
  for (auto &ui : mUi) {
    ui.second->BeginDraw();
    ui.second->EndDraw();
  }
}

Error UIManager::AddChildToUi(int parentId, int childId,
                              std::unique_ptr<UIBase> ui) {
  if (!mUi.contains(parentId)) {
    return MakeError("Parent key does not exist in ui");
  }
  if (mChildren.contains(childId)) {
    return MakeError("Child key does already exist");
  }
  auto &parent = mUi[parentId];

  auto [result, _] =
      mChildren.emplace(childId, ChildInfo(parentId, std::move(ui)));

  auto childPtr = result->second.childUi.get();
  parent->AddChild(childPtr);

  return Error::Success();
}

} // namespace GUILib
