#pragma once

#include "Parser/DOL.h"

#include <cinttypes>
#include <vector>

#include <capstone/capstone.h>
#include <capstone/platform.h>

#include <ostream>

namespace Disassembler {

void DisassemblePPC(Patcher::Parsing::DOLFile &dol);
void DisassembleSection(std::uint32_t offset, std::uint32_t address,
                        std::uint32_t size, std::vector<std::uint8_t> &data);

void PrintInstruction(cs_insn *ins, std::ostream &os, bool details = false);

} // namespace Disassembler

void test();
