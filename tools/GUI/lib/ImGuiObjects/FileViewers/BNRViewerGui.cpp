#include "ImGuiObjects/FileViewers/BNRViewerGui.h"

// GCToolkit
#include <Parser/BNR.h>
#include <Support/DiskSingleton.h>

// Util
#include <Logger/Logger.h>

// ImGui
#include <imgui.h>

// stl
#include <fstream>
#include <sstream>

namespace UIObjects {

BNRViewerGui::BNRViewerGui() : FileViewerBase() { mExtension = ".bnr"; }

void BNRViewerGui::OnInit() {}

void BNRViewerGui::OnBeginDraw() {
  ImGui::BeginChild("BNR Viewer");
  const auto &projectDir = Support::DiskSingleton::GetSingleton().ProjectDir;
  ImGui::Text("%s", std::filesystem::relative(mFile, projectDir).c_str());
  ImGui::Separator();
  ImGui::BeginChild("BNR Header Viewer",
                    ImVec2(0, ImGui::GetContentRegionAvail().y - 30));
  ImGui::TextUnformatted(mBnrHeader.c_str());
  ImGui::EndChild();
  ImGui::Separator();
}

void BNRViewerGui::OnEndDraw() { ImGui::EndChild(); }

void BNRViewerGui::OnSetFile() {
  Parsing::BNRFile bnr;
  std::ifstream input(mFile.c_str(), std::ios::binary);
  bnr.ParseHeader(input);
  std::stringstream sstream;
  bnr.PrintHeader(sstream);
  mBnrHeader = sstream.str();
}

} // namespace UIObjects
