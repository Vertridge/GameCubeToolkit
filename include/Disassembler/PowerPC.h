#pragma once

#include "Disassembler/Program.h"
#include "Parser/DOL.h"

#include <cinttypes>
#include <vector>

#include <capstone/capstone.h>
#include <capstone/platform.h>

#include <array>
#include <ostream>

namespace PowerPC {
class Program;
class TextSection;

namespace Disassembler {

Program DisassemblePPC(Patcher::Parsing::DOLFile &dol, std::ostream &os);

TextSection DisassembleSection(std::uint32_t offset, std::uint32_t address,
                               std::uint32_t size,
                               std::vector<std::uint8_t> &data,
                               std::ostream &os);
void PrintInstruction(cs_insn *ins, std::ostream &os, bool details = false);

} // namespace Disassembler
} // namespace PowerPC
