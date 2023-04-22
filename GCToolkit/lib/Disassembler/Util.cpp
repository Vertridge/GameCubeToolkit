#include "Disassembler/Util.h"

#include <algorithm>

bool PowerPC::Util::IsBranchInstruction(ppc_insn instr) {
  return std::find(branch_instructions.begin(), branch_instructions.end(),
                   instr) != branch_instructions.end();
}

bool PowerPC::Util::IsCallInstruction(ppc_insn instr) {
  return std::find(call_instructions.begin(), call_instructions.end(), instr) !=
         call_instructions.end();
}
