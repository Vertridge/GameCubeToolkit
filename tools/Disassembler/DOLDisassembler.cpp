#include <fstream>
#include <iostream>

#include <Parser/DOL.h>

#include "Disassembler/PowerPC.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Pass 2 file to patch" << std::endl;
    return 1;
  }

  std::cout << "DOLDisassembler: " << argv[1] << std::endl;

  Patcher::Parsing::DOLFile dol;
  if (!dol.Parse(argv[1])) {
    std::cerr << "Failed to open:" << argv[1] << std::endl;
    return 1;
  }

  std::string output = "disassemble.txt";
  if (argc >= 3) {
    output = argv[2];
  }

  std::ofstream ofstrm(output, std::ios::binary);

  dol.PrintHeader(ofstrm);

  auto program = PowerPC::Disassembler::DisassemblePPC(dol, ofstrm);

  std::string output2 = "dump.txt";
  std::ofstream dmpstrm(output2, std::ios::binary);

  program.Dump(dmpstrm);

  return 0;
}
