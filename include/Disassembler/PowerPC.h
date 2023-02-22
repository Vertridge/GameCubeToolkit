#pragma once

#include "Parser/DOL.h"

#include <cinttypes>
#include <vector>

#include <capstone/capstone.h>
#include <capstone/platform.h>

#include <ostream>

namespace PowerPC::Disassembler {

void DisassemblePPC(Patcher::Parsing::DOLFile &dol, std::ostream &os);

void DisassembleSection(std::uint32_t offset, std::uint32_t address,
                        std::uint32_t size, std::vector<std::uint8_t> &data,
                        std::ostream &os);
void PrintInstruction(cs_insn *ins, std::ostream &os, bool details = false);

} // namespace PowerPC::Disassembler

void test();
