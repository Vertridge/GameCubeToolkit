#include "Disassembler/PowerPC.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <stdio.h>

struct platform {
  cs_arch arch;
  cs_mode mode;
  unsigned char *code;
  size_t size;
  const char *comment;
};

static csh handle;

static void print_string_hex(const char *comment, unsigned char *str,
                             size_t len) {
  unsigned char *c;

  printf("%s", comment);
  for (c = str; c < str + len; c++) {
    printf("0x%02x ", *c & 0xff);
  }

  printf("\n");
}

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

static void print_insn_detail(cs_insn *ins) {
  cs_ppc *ppc;
  int i;

  // detail can be NULL on "data" instruction if SKIPDATA option is turned ON
  if (ins->detail == NULL)
    return;

  ppc = &(ins->detail->ppc);
  if (ppc->op_count)
    printf("\top_count: %u\n", ppc->op_count);

  for (i = 0; i < ppc->op_count; i++) {
    cs_ppc_op *op = &(ppc->operands[i]);
    switch ((int)op->type) {
    default:
      break;
    case PPC_OP_REG:
      printf("\t\toperands[%u].type: REG = %s\n", i,
             cs_reg_name(handle, op->reg));
      break;
    case PPC_OP_IMM:
      printf("\t\toperands[%u].type: IMM = 0x%" PRIx64 "\n", i, op->imm);
      break;
    case PPC_OP_MEM:
      printf("\t\toperands[%u].type: MEM\n", i);
      if (op->mem.base != PPC_REG_INVALID)
        printf("\t\t\toperands[%u].mem.base: REG = %s\n", i,
               cs_reg_name(handle, op->mem.base));
      if (op->mem.disp != 0)
        printf("\t\t\toperands[%u].mem.disp: 0x%x\n", i, op->mem.disp);

      break;
    case PPC_OP_CRX:
      printf("\t\toperands[%u].type: CRX\n", i);
      printf("\t\t\toperands[%u].crx.scale: %d\n", i, op->crx.scale);
      printf("\t\t\toperands[%u].crx.reg: %s\n", i,
             cs_reg_name(handle, op->crx.reg));
      printf("\t\t\toperands[%u].crx.cond: %s\n", i, get_bc_name(op->crx.cond));
      break;
    }
  }

  if (ppc->bc != 0)
    printf("\tBranch code: %u\n", ppc->bc);

  if (ppc->bh != 0)
    printf("\tBranch hint: %u\n", ppc->bh);

  if (ppc->update_cr0)
    printf("\tUpdate-CR0: True\n");

  printf("\n");
}

void test() {
#define PPC_CODE                                                               \
  "\x43\x20\x0c\x07\x41\x56\xff\x17\x80\x20\x00\x00\x80\x3f\x00\x00\x10\x43"   \
  "\x23\x0e\xd0\x44\x00\x80\x4c\x43\x22\x02\x2d\x03\x00\x80\x7c\x43\x20\x14"   \
  "\x7c\x43\x20\x93\x4f\x20\x00\x21\x4c\xc8\x00\x21\x40\x82\x00\x14"
#define PPC_CODE2 "\x10\x60\x2a\x10\x10\x64\x28\x88\x7c\x4a\x5d\x0f"

  struct platform platforms[] = {
      {
          CS_ARCH_PPC,
          CS_MODE_BIG_ENDIAN,
          (unsigned char *)PPC_CODE,
          sizeof(PPC_CODE) - 1,
          "PPC-64",
      },
      {
          CS_ARCH_PPC,
          (cs_mode)(CS_MODE_BIG_ENDIAN + CS_MODE_QPX),
          (unsigned char *)PPC_CODE2,
          sizeof(PPC_CODE2) - 1,
          "PPC-64 + QPX",
      },
  };

  uint64_t address = 0x1000;
  cs_insn *insn;
  std::size_t i;
  size_t count;

  for (i = 0; i < sizeof(platforms) / sizeof(platforms[0]); i++) {
    cs_err err = cs_open(platforms[i].arch, platforms[i].mode, &handle);
    if (err) {
      printf("Failed on cs_open() with error returned: %u\n", err);
      abort();
    }

    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

    count = cs_disasm(handle, platforms[i].code, platforms[i].size, address, 0,
                      &insn);
    if (count) {
      size_t j;

      printf("****************\n");
      printf("Platform: %s\n", platforms[i].comment);
      print_string_hex("Code:", platforms[i].code, platforms[i].size);
      printf("Disasm:\n");

      for (j = 0; j < count; j++) {
        printf("0x%" PRIx64 ":\t%s\t%s\n", insn[j].address, insn[j].mnemonic,
               insn[j].op_str);
        print_insn_detail(&insn[j]);
      }
      printf("0x%" PRIx64 ":\n", insn[j - 1].address + insn[j - 1].size);

      // free memory allocated by cs_disasm()
      cs_free(insn, count);
    } else {
      printf("****************\n");
      printf("Platform: %s\n", platforms[i].comment);
      print_string_hex("Code:", platforms[i].code, platforms[i].size);
      printf("ERROR: Failed to disasm given code!\n");
      abort();
    }

    printf("\n");

    cs_close(&handle);
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

  os << std::endl;
  // printf("0x%" PRIx64 ":\t%s\t%s\n", insn->address, insn->mnemonic,
  //       insn->op_str);
  // print_insn_detail(&insn[j]);
}

void Disassembler::DisassembleSection(std::uint32_t offset,
                                      std::uint32_t address, std::uint32_t size,
                                      std::vector<std::uint8_t> &data) {
  if (size == 0) {
    return;
  }
  std::ofstream ofstrm("test.txt", std::ios::binary);
  cs_insn *insn;
  auto start = address;
  auto end = start + size;
  std::size_t codeStart = offset - sizeof(Patcher::Parsing::DOLHeader);
  auto *code = data.data() + codeStart;
  std::cout << std::hex << code << " " << *code << std::endl;
  size_t code_size = size;
  size_t addr = address;
  size_t totalCount = 0;
  while (addr < end) {
    auto count =
        cs_disasm(handle, code, code_size, addr, size - codeStart, &insn);
    if (!count) {
      printf("Error: failed at %ld : size: %d\n", addr, size);
      addr += sizeof(std::uint32_t);
      code += sizeof(std::uint32_t);
      continue;
    }
    if (cs_errno(handle) != CS_ERR_OK) {
      printf("Error: %s\n", cs_strerror(cs_errno(handle)));
      break;
    }

    for (std::size_t j = 0; j < count; j++) {
      PrintInstruction(&insn[j], ofstrm, /*details*/ false);
    }
    cs_free(insn, count);
    totalCount += count;
    printf("Count: %ld / %ld\n", count, totalCount);
    addr += count * sizeof(std::uint32_t);
    code += count * sizeof(std::uint32_t);
  }
}

void Disassembler::DisassemblePPC(Patcher::Parsing::DOLFile &dol) {

  cs_err err = cs_open(CS_ARCH_PPC, CS_MODE_BIG_ENDIAN, &handle);
  if (err) {
    printf("Failed on cs_open() with error returned: %u\n", err);
    abort();
  }

  cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

  for (std::size_t i = 0; i < Patcher::Parsing::DOLNumberOfTextSections; ++i) {
    DisassembleSection(dol.GetHeader()->TextSectionOffsets[i],
                       dol.GetHeader()->TextSectionAddresses[i],
                       dol.GetHeader()->TextSectionSizes[i], dol.GetData());
  }

  // uint64_t address = 0x1000;
  // cs_insn *insn;
  // size_t count;

  // struct platform platforms = {
  //     CS_ARCH_PPC,          CS_MODE_BIG_ENDIAN, (unsigned char *)PPC_CODE,
  //     sizeof(PPC_CODE) - 1, "PPC-64",
  // };

  // cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

  // count = cs_disasm(handle, data.data(), data.size(), address, 0, &insn);
  // if (count) {
  //   printf("****************\n");
  //   printf("Platform: %s\n", "PPC-64");
  //   // print_string_hex("Code:", data.data(), data.size());
  //   printf("Disasm:\n");
  //   printf("Count: %ld \n", count);

  //   std::size_t j = 0;

  //   for (j = 0; j < count; j++) {
  //     printf("0x%" PRIx64 ":\t%s\t%s\n", insn[j].address, insn[j].mnemonic,
  //            insn[j].op_str);
  //     print_insn_detail(&insn[j]);
  //   }
  //   printf("0x%" PRIx64 ":\n", insn[j - 1].address + insn[j - 1].size);

  //   // free memory allocated by cs_disasm()
  //   cs_free(insn, count);
  // } else {
  //   printf("****************\n");
  //   printf("Platform: %s\n", "PPC-64");
  //   print_string_hex("Code:", data.data(), data.size());
  //   printf("ERROR: Failed to disasm given code!\n");
  //   abort();
  // }

  printf("\n");

  cs_close(&handle);
}
