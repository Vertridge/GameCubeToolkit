#include "ImGuiObjects/UnpackerGui.h"

// vendor
#include <imgui.h>

namespace UIObjects {

void UnpackerGui::OnInit() {}

void UnpackerGui::OnBeginDraw() {
  ImGui::Begin("Unpacker");

  ImGui::Button("Unpack");
}

void UnpackerGui::OnEndDraw() { ImGui::End(); }

} // namespace UIObjects
