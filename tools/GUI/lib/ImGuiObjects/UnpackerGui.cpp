#include "ImGuiObjects/UnpackerGui.h"

#include <Support/DiskSingleton.h>
#include <Unpacker/Unpacker.h>

// vendor
#include <imgui.h>

// stl
#include <iostream>

namespace UIObjects {

namespace {
void Unpack() {
  auto &disk = Support::DiskSingleton::GetSingleton();
  if (disk.ProjectDir.empty()) {
    Support::ChooseProjectDir();
  }
  std::cout << "output dir: " << disk.ProjectDir << std::endl;

  Unpacking::UnpackerOptions options{disk.Iso, disk.Dump, disk.ProjectDir};

  Unpacking::Unpacker unpacker(options);
  unpacker.Dump();
  if (!unpacker.Unpack()) {
    std::cerr << "Failed to unpack " << disk.Iso << std::endl;
  }
}

} // namespace

void UnpackerGui::OnInit() {}

void UnpackerGui::OnBeginDraw() {
  ImGui::Begin("Unpacker");

  if (ImGui::Button("Unpack")) {
    Unpack();
  }
  ImGui::Separator();
}

void UnpackerGui::OnEndDraw() { ImGui::End(); }

} // namespace UIObjects
