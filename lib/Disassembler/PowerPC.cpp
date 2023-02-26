#include "Disassembler/PowerPC.h"

#include "Disassembler/Program.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <stdio.h>

using namespace PowerPC;

constexpr std::array<ppc_insn, 6> BranchInstructions = {
    PPC_INS_B, PPC_INS_BL, PPC_INS_BC, PPC_INS_BDZ, PPC_INS_BDNZ};

static csh handle;

static const char *get_bc_name(int bc) {
  switch (bc) {
  default:
  case PPC_BC_INVALID:
    return ("invalid");
  case PPC_BC_LT:
    return ("lt");
  case PPC_BC_LE:
    return ("le");
  case PPC_BC_EQ:
    return ("eq");
  case PPC_BC_GE:
    return ("ge");
  case PPC_BC_GT:
    return ("gt");
  case PPC_BC_NE:
    return ("ne");
  case PPC_BC_UN:
    return ("un");
  case PPC_BC_NU:
    return ("nu");
  case PPC_BC_SO:
    return ("so");
  case PPC_BC_NS:
    return ("ns");
  }
}

void Disassembler::PrintInstruction(cs_insn *ins, std::ostream &os,
                                    bool details) {
  // printf("Is this function executed?");
  os << "0x" << std::hex << ins->address << "\t" << ins->mnemonic << "\t"
     << ins->op_str << "\n";

  if (!details) {
    return;
  }

  // detail can be NULL on "data" instruction if SKIPDATA option is turned
  // ON
  if (ins->detail == NULL) {

    return;
  }

  cs_ppc *ppc;
  int i;

  ppc = &(ins->detail->ppc);
  if (ppc->op_count) {
    os << "\top_count: " << +ppc->op_count << "\n";
  }

  for (i = 0; i < ppc->op_count; i++) {
    auto *op = &(ppc->operands[i]);
    switch ((int)op->type) {
    default:
      break;
    case PPC_OP_REG:
      os << "\t\toperands[" << i
         << "].type: REG = " << cs_reg_name(handle, op->reg) << "\n";
      break;
    case PPC_OP_IMM:
      os << "\t\toperands[" << i << "].type: IMM = 0x" << std::hex << op->imm
         << "\n";
      break;
    case PPC_OP_MEM:
      os << "\t\toperands[" << i << "].type: MEM\n";
      if (op->mem.base != PPC_REG_INVALID) {
        os << "\t\toperands[" << i
           << "].mem.base: REG =" << cs_reg_name(handle, op->mem.base) << "\n";
      }
      if (op->mem.disp != 0) {
        os << "\t\toperands[" << i << "].mem.disp: 0x " << std::hex
           << op->mem.disp << "\n";
      }
      break;
    case PPC_OP_CRX:
      os << "\t\toperands[" << i << ".type: CRX\n";
      os << "\t\toperands[" << i << "crx.scale: " << op->crx.scale << "\n";
      os << "\t\toperands[" << i
         << ".crx.reg: " << cs_reg_name(handle, op->crx.reg) << "\n";
      os << "\t\toperands[" << i << ".crx.cond: " << get_bc_name(op->crx.cond)
         << "\n";
      break;
    }
  }

  if (ppc->bc != 0) {
    os << "\tBranch code: " << ppc->bc << "\n";
  }

  if (ppc->bh != 0) {
    os << "\tBranch hint: " << ppc->bh << "\n";
  }

  if (ppc->update_cr0) {
    os << "\tUpdate-CR0: True\n";
  }

  os << "\n";
}

std::shared_ptr<Function> CreateFunction(std::uint32_t address) {
  std::stringstream stream;
  stream << "function_0x" << std::hex << address;

  return std::make_shared<Function>(stream.str(), address);
}

TextSection Disassembler::DisassembleSection(std::uint32_t offset,
                                             std::uint32_t address,
                                             std::uint32_t size,
                                             std::vector<std::uint8_t> &data,
                                             std::ostream &os) {

  TextSection textSection(offset, address, size);

  if (size == 0) {
    return textSection;
  }

  cs_insn *insn;
  auto start = address;
  auto end = start + size;
  std::size_t codeStart = offset - sizeof(Patcher::Parsing::DOLHeader);
  auto *code = data.data() + codeStart;
  size_t code_size = size;
  size_t addr = address;
  size_t totalCount = 0;
  auto &instructions = textSection.GetInstructions();
  while (addr < end) {
    auto count = cs_disasm(handle, code, code_size, addr, 1, &insn);

    if (!count) {
      os << "0x" << std::hex << addr << "\t"
         << "0x" << +(*code) << "\n";
      addr += sizeof(std::uint32_t);
      code += sizeof(std::uint32_t);
      continue;
    }
    if (cs_errno(handle) != CS_ERR_OK) {
      printf("Error: %s\n", cs_strerror(cs_errno(handle)));
      break;
    }

    for (std::size_t j = 0; j < count; j++) {
      PrintInstruction(&insn[j], os, /*details*/ false);
      std::stringstream instr;
      instr << "\t" << insn[j].mnemonic << "\t" << insn[j].op_str;

      instructions.emplace_back(
          std::make_shared<Instruction>(instr.str(), addr));
      if (insn[j].id == PPC_INS_BL) {
        if (insn->detail == NULL) {
          continue;
        }
        cs_ppc *ppc = &(insn->detail->ppc);
        for (auto i = 0; i < ppc->op_count; i++) {
          auto *op = &(ppc->operands[i]);
          if ((int)op->type != PPC_OP_IMM) {
            continue;
          }

          std::uint32_t funcAddr = static_cast<std::uint32_t>(op->imm);
          if (textSection.HasFunctionAt(funcAddr) ||
              !textSection.AddressInSection(funcAddr)) {
            continue;
          }

          auto func = CreateFunction(funcAddr);
          textSection.AddFunction(funcAddr, std::move(func));
        }
      }
    }
    cs_free(insn, count);
    totalCount += count;

    addr += count * sizeof(std::uint32_t);
    code += count * sizeof(std::uint32_t);
  }
  return textSection;
}

Program Disassembler::DisassemblePPC(Patcher::Parsing::DOLFile &dol,
                                     std::ostream &os) {

  cs_err err = cs_open(CS_ARCH_PPC, CS_MODE_BIG_ENDIAN, &handle);
  if (err) {
    printf("Failed on cs_open() with error returned: %u\n", err);
    abort();
  }

  Program program;
  program.SetName(dol.GetFileName());

  cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

  for (std::size_t i = 0; i < Patcher::Parsing::DOLNumberOfTextSections; ++i) {
    os << "Text Section[" << i << "]\t"
       << "offset 0x" << std::hex << dol.GetHeader()->TextSectionOffsets[i]
       << "\taddress 0x" << dol.GetHeader()->TextSectionAddresses[i]
       << "\tsize 0x" << dol.GetHeader()->TextSectionSizes[i] << "\n";

    auto textSection = DisassembleSection(
        dol.GetHeader()->TextSectionOffsets[i],
        dol.GetHeader()->TextSectionAddresses[i],
        dol.GetHeader()->TextSectionSizes[i], dol.GetData(), os);
    textSection.SetName("TextSection_" + std::to_string(i));
    ParseInstructionToFunctions(textSection);
    program.AddTextSection(std::move(textSection));
  }

  cs_close(&handle);

  return program;
}

void PowerPC::ParseInstructionToFunctions(TextSection &section) {
  auto &instructions = section.GetInstructions();
  std::shared_ptr<Function> &tempfunction = section.begin()->second;
  for (auto &instr : instructions) {
    if (section.HasFunctionAt(instr->GetAddress())) {
      tempfunction = section.GetFunction(instr->GetAddress());
    }
    instr->SetParent(tempfunction);
    tempfunction->Add(instr);
  }
}
