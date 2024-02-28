#include "ImGuiObjects/PackageViewerGui.h"

#include <Logger/Logger.h>
#include <Support/DiskSingleton.h>

// stl
#include <filesystem>

// vendor
#include <imgui.h>

namespace UIObjects {

void PackageViewerGui::OnInit() {}

void PackageViewerGui::DrawFileSystem() {
  const auto &projectDir = Support::DiskSingleton::GetSingleton().ProjectDir;
  if (projectDir.empty()) {
    return;
  }
  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(projectDir)) {
    ImGui::Selectable(
        std::filesystem::relative(entry.path(), projectDir).string().c_str());
  }
}

void PackageViewerGui::OnBeginDraw() {
  ImGui::BeginChild("Package Viewer");
  DrawFileSystem();
}

void PackageViewerGui::OnEndDraw() { ImGui::EndChild(); }

} // namespace UIObjects
