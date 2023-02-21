#include <iostream>

#include <Parser/DOL.h>

#include "Disassembler/PowerPC.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Pass 2 file to patch" << std::endl;
    return 1;
  }

  std::cout << "DOLPatcher: " << argv[1] << std::endl;

  Patcher::Parsing::DOLFile dol;
  if (!dol.Parse(argv[1])) {
    std::cerr << "Failed to open:" << argv[1] << std::endl;
    return 1;
  }
  dol.PrintHeader();

  Disassembler::DisassemblePPC(dol);

  return 0;
}
