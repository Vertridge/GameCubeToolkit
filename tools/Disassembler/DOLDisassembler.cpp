#include <fstream>
#include <iostream>

#include <Parser/DOL.h>

#include <Disassembler/PowerPC.h>

// Vendor
#include "cxxopts.hpp"

int main(int argc, char *argv[]) {

  cxxopts::Options options("DOLDisassembler", "Disassemble DOL");

  // clang-format off
  options.add_options()
      ("dump,dump-file", "Dump file", cxxopts::value<std::string>())
      ("dis,disassembly", "Disassenbly file", cxxopts::value<std::string>())
      ("h,help", "Print usage")
      ("dol", "Input", cxxopts::value<std::string>());
  // clang-format on

  options.parse_positional({"dol"});

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (!result.count("dol")) {
    std::cout << "dol file to patch required\n" << options.help() << std::endl;
    return 1;
  }

  std::string input = result["dol"].as<std::string>();

  std::cout << "DOLDisassembler: " << input << std::endl;

  Parsing::DOLFile dol;
  if (!dol.Parse(input)) {
    std::cerr << "Failed to open: " << input << std::endl;
    return 1;
  }

  std::string output = "disassembly.txt";
  if (result.count("dis")) {
    output = result["disassembly"].as<std::string>();
  }

  std::ofstream ofstrm(output, std::ios::binary);

  dol.PrintHeader(ofstrm);

  std::cout << "Disassembling" << std::endl;
  auto program = PowerPC::Disassembler::DisassemblePPC(dol, ofstrm);

  std::string dump = "dump.txt";
  if (result.count("dump")) {
    dump = result["dump"].as<std::string>();
  }

  std::ofstream dmpstrm(dump, std::ios::binary);

  std::cout << "Dumping" << std::endl;
  program.Dump(dmpstrm);

  return 0;
}
