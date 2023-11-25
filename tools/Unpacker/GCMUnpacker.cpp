#include <filesystem>
#include <fstream>
#include <iostream>

#include "Parser/FST.h"
#include "Parser/GCM.h"
#include "Unpacker/Unpacker.h"

// Vendor
#include "cxxopts.hpp"

int Unpack(cxxopts::ParseResult &result) {
  if (!result.count("gcm")) {
    std::cout << "gcm is required when unpacking see help" << std::endl;
    return 1;
  }

  auto input = result["gcm"].as<std::string>();

  std::cout << "GCM Unpacker: " << input << std::endl;

  std::string dump = "gcmDump.txt";
  if (result.count("dump")) {
    dump = result["dump"].as<std::string>();
  }

  std::filesystem::path out = "out";
  if (result.count("output")) {
    out = result["output"].as<std::string>();
  }

  Unpacking::Unpacker unpacker({input, dump, out});

  unpacker.Dump();

  if (!unpacker.Unpack()) {
    std::cerr << "Failed to unpack " << input << std::endl;
    return 1;
  }

  return 0;
}

int Repack(cxxopts::ParseResult &result) {
  std::cerr << "Repacking is currently not supported" << std::endl;

  std::string out = "./out";
  if (result.count("output")) {
    out = result["output"].as<std::string>();
  }

  return 1;
}

int main(int argc, char *argv[]) {

  cxxopts::Options options("GCMUnpacker", "Unpack GCM / iso");

  // clang-format off
  options.add_options()
      ("o,output", "Output directory", cxxopts::value<std::string>())
      ("d,dump-file", "Dump file", cxxopts::value<std::string>())
      ("u,unpack", "Unpack")
      ("p,pack", "Repack")
      ("h,help", "Print usage")
      ("g,gcm", "Input", cxxopts::value<std::string>());
  // clang-format on

  options.parse_positional({"gcm"});

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (result.count("pack") == 0 && result.count("unpack") == 0) {
    std::cout << "Pack or unpack is required\n" << options.help() << std::endl;
    return 0;
  }

  if (result.count("unpack")) {
    return Unpack(result);
  } else if (result.count("pack")) {
    return Repack(result);
  }
  return 0;
}
