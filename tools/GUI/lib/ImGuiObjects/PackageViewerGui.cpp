#include "ImGuiObjects/PackageViewerGui.h"

#include <Logger/Logger.h>
#include <Support/DiskSingleton.h>

#include "ImGuiObjects/FileViewers/DOLViewerGui.h"
#include "ImGuiObjects/FileViewers/TxtViewerGui.h"

// stl
#include <filesystem>

// vendor
#include <imgui.h>

namespace UIObjects {

void PackageViewerGui::OnInit() {
  mFileViewers.emplace_back(std::make_unique<DOLViewerGui>());
  mFileViewers.emplace_back(std::make_unique<TxtViewerGui>());
}

void PackageViewerGui::OnFileSelected(const std::filesystem::path &path) {
  if (currentViewer != nullptr) {
    currentViewer->SetEnabled(false);
  }
  for (auto &viewer : mFileViewers) {
    if (viewer->SetFile(path)) {
      currentViewer = viewer.get();
      currentViewer->SetEnabled(true);
      break;
    }
  }
}

void PackageViewerGui::DrawFileSystem() {
  const auto &projectDir = Support::DiskSingleton::GetSingleton().ProjectDir;
  if (projectDir.empty()) {
    return;
  }
  static int selectedIndex = -1;
  int i = 0;
  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(projectDir)) {
    if (entry.is_directory()) {
      continue;
    }
    if (ImGui::Selectable(
            std::filesystem::relative(entry.path(), projectDir).c_str(),
            selectedIndex == i)) {
      selectedIndex = i;
      OnFileSelected(entry.path());
    }
    ++i;
  }
}

void PackageViewerGui::OnBeginDraw() {
  ImGui::BeginChild("Package Viewer");
  ImGui::BeginChild("FileSystem",
                    ImVec2(ImGui::GetContentRegionAvail().x / 2 - 20, 0));
  DrawFileSystem();
  ImGui::EndChild();
  ImGui::SameLine();
  if (currentViewer != nullptr) {
    currentViewer->BeginDraw();
    currentViewer->EndDraw();
  }
}

void PackageViewerGui::OnEndDraw() { ImGui::EndChild(); }

} // namespace UIObjects
