#include "ImGuiObjects/FileViewers/TxtViewerGui.h"

#include <Logger/Logger.h>
#include <Support/DiskSingleton.h>

// ImGui
#include <imgui.h>

// stl
#include <fstream>
#include <sstream>

namespace UIObjects {

TxtViewerGui::TxtViewerGui() : FileViewerBase() { mExtension = ".txt"; }

void TxtViewerGui::OnInit() {}

void TxtViewerGui::OnBeginDraw() {
  ImGui::BeginChild("Txt Viewer");
  const auto &projectDir = Support::DiskSingleton::GetSingleton().ProjectDir;
  ImGui::Text("%s", std::filesystem::relative(mFile, projectDir).c_str());
  ImGui::Separator();
  ImGui::BeginChild("DOL Header Viewer",
                    ImVec2(0, ImGui::GetContentRegionAvail().y));
  ImGui::Text("%s", mTxtContent.c_str());
  ImGui::EndChild();
}

void TxtViewerGui::OnEndDraw() { ImGui::EndChild(); }

void TxtViewerGui::OnSetFile() {
  std::ifstream input(mFile.c_str());
  input.seekg(0, std::ios::end);
  size_t size = input.tellg();
  input.seekg(0, std::ios::beg);
  mTxtContent.reserve(size);
  mTxtContent.assign((std::istreambuf_iterator<char>(input)),
                     std::istreambuf_iterator<char>());
}

} // namespace UIObjects
