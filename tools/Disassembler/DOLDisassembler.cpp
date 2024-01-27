#include <fstream>
#include <iostream>

#include <Parser/DOL.h>

#include <Disassembler/PowerPC.h>

// Util
#include <Logger/ConsoleLogger.h>
#include <Logger/Logger.h>

// Vendor
#include "cxxopts.hpp"

void InitLogger() {
  auto &logger = util::Logger::GetSingleton();
  logger.AddLogger(new util::ConsoleLogger());
}

int main(int argc, char *argv[]) {
  InitLogger();

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
    LOG_INFO(options.help());
    return 0;
  }

  if (!result.count("dol")) {
    LOG_ERROR("dol file to patch required");
    LOG_ERROR(options.help());
    return 1;
  }

  std::string input = result["dol"].as<std::string>();

  LOG_INFO("DOLDisassembler: {}", input);

  Parsing::DOLFile dol;
  if (!dol.Parse(input)) {
    LOG_ERROR("Failed to open: {}", input);
    return 1;
  }

  std::string output = "disassembly.txt";
  if (result.count("dis")) {
    output = result["disassembly"].as<std::string>();
  }

  std::ofstream ofstrm(output, std::ios::binary);

  dol.PrintHeader(ofstrm);

  LOG_INFO("Disassembling");
  auto program = PowerPC::Disassembler::DisassemblePPC(dol, ofstrm);

  LOG_INFO("Parsing functions and blocks");
  PowerPC::ParseInstructionToFunctions(program);

  std::string dump = "dump.txt";
  if (result.count("dump")) {
    dump = result["dump"].as<std::string>();
  }

  std::ofstream dmpstrm(dump, std::ios::binary);

  LOG_INFO("Dumping");
  program.Dump(dmpstrm);

  return 0;
}
