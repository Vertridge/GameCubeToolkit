#include "ImGuiObjects/FileViewers/DOLViewerGui.h"

// GCToolkit
#include <GUIProgram.h>
#include <ImGuiObjects/DOLDisassemblerGUI.h>
#include <Parser/DOL.h>
#include <Support/DiskSingleton.h>

// Util
#include <Logger/Logger.h>

// ImGui
#include <imgui.h>

// stl
#include <fstream>
#include <memory>
#include <sstream>

namespace UIObjects {

DOLViewerGui::DOLViewerGui() : FileViewerBase() { mExtension = ".dol"; }

void DOLViewerGui::OnInit() {}

void DOLViewerGui::OnBeginDraw() {
  ImGui::BeginChild("DOL Viewer");
  const auto &projectDir = Support::DiskSingleton::GetSingleton().ProjectDir;
  ImGui::Text("%s", std::filesystem::relative(mFile, projectDir).c_str());
  ImGui::Separator();
  ImGui::BeginChild("DOL Header Viewer",
                    ImVec2(0, ImGui::GetContentRegionAvail().y - 30));
  ImGui::Text("%s", dolHeader.c_str());
  ImGui::EndChild();
  ImGui::Separator();
  if (ImGui::Button("Disassemble")) {
    LOG_INFO("Disassembling!");

    GUI::GUIProgram::GetInstance()->GetUiManager().AddUi(
        DOLDisassemblerGui::GetID(),
        std::make_unique<DOLDisassemblerGui>(mFile));
  }
}

void DOLViewerGui::OnEndDraw() { ImGui::EndChild(); }

void DOLViewerGui::OnSetFile() {
  Parsing::DOLFile dol;
  std::ifstream input(mFile.c_str(), std::ios::binary);
  dol.ParseHeader(input);
  std::stringstream sstream;
  dol.PrintHeader(sstream);
  dolHeader = sstream.str();
}

} // namespace UIObjects
