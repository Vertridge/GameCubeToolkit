#pragma once

// GCToolkit
#include "Disassembler/Program.h"
#include "ImGui/UIBase.h"
#include "ImGuiObjects/UI_ID.h"

#include <Disassembler/PowerPC.h>
#include <Parser/DOL.h>

// stl
#include <filesystem>

using namespace GUILib;

namespace UIObjects {

class DOLDisassemblerGui : public UIBase {
public:
  ID(UI_ID::DOL_DISASSEMBLER_UI);

  DOLDisassemblerGui(const std::filesystem::path &path);

  void OnInit() override;
  void OnBeginDraw() override;
  void OnEndDraw() override;

private:
  const std::filesystem::path mDolPath;
  Parsing::DOLFile mDol;
  PowerPC::Program mProgram;
};

} // namespace UIObjects
