#include "ImGui/UIManager.h"

namespace GUILib {

void UIManager::Init() {
  for (auto &ui : mUi) {
    ui.second->Init();
  }
}

Error UIManager::AddUi(int id, std::unique_ptr<UIBase> ui) {
  if (mUi.find(id) != mUi.end()) {
    return MakeError("Key already exists in map");
  }
  mUi.emplace(id, std::move(ui));
  return Error::Success();
}

Error UIManager::RemoveUi(int id) {
  auto ui = mUi.find(id);
  if (ui == mUi.end()) {
    return MakeError("Key does exists in map");
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

} // namespace GUILib
