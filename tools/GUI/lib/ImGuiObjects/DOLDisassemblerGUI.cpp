#include "ImGuiObjects/DOLDisassemblerGUI.h"
#include "Disassembler/Program.h"

#include <Support/DiskSingleton.h>

// Util
#include <Logger/Logger.h>

// vendor
#include <imgui.h>

// stl
#include <fstream>
#include <sstream>

using namespace GUILib;

namespace UIObjects {

DOLDisassemblerGui::DOLDisassemblerGui(const std::filesystem::path &path)
    : mDolPath(path) {}

void DOLDisassemblerGui::OnInit() {
  if (!mDol.Parse(mDolPath)) {
    LOG_ERROR("Failed to parse dol at: {}", mDolPath.string());
    return;
  }
  std::ofstream ofstrm(Support::DiskSingleton::GetSingleton().Disassembly,
                       std::ios::binary);

  LOG_INFO("Disassembling");
  mProgram = PowerPC::Disassembler::DisassemblePPC(mDol, ofstrm);
  LOG_INFO("Disassembly written to {}",
           Support::DiskSingleton::GetSingleton().Disassembly.string());
  PowerPC::ParseInstructionToFunctions(mProgram);
  LOG_INFO("Disassembling Done!");
  std::stringstream sstream;
  mProgram.Dump(sstream);
  mProgramDump = std::move(sstream).str();
}

void DOLDisassemblerGui::OnBeginDraw() {
  ImGui::Begin("DOL Disassembly");

  ImGui::Text("%s", mProgramDump.c_str());
}

void DOLDisassemblerGui::OnEndDraw() { ImGui::End(); }

} // namespace UIObjects
