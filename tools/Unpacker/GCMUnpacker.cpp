#include <filesystem>
#include <fstream>
#include <iostream>

#include "Parser/FST.h"
#include "Parser/GCM.h"
#include "Unpacker/Unpacker.h"

// Util
#include <Logger/ConsoleLogger.h>
#include <Logger/Logger.h>

// Vendor
#include "cxxopts.hpp"

int Unpack(cxxopts::ParseResult &result) {
  if (!result.count("gcm")) {
    LOG_ERROR("'gcm' is required when unpacking see help");
    return 1;
  }

  auto input = result["gcm"].as<std::string>();

  LOG_INFO("GCM Unpacker: {}", input);

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
    LOG_ERROR("Failed to unpack {}", input);
    return 0;
  }
  LOG_INFO("Unpacking done");

  return 0;
}

int Repack(cxxopts::ParseResult &result) {
  LOG_ERROR("Repacking is currently not supported");

  std::string out = "./out";
  if (result.count("output")) {
    out = result["output"].as<std::string>();
  }

  return 1;
}

void InitLogger() {
  auto &logger = util::Logger::GetSingleton();
  logger.AddLogger(new util::ConsoleLogger());
}

int main(int argc, char *argv[]) {

  InitLogger();

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
    LOG_INFO(options.help());
    return 0;
  }

  if (result.count("pack") == 0 && result.count("unpack") == 0) {
    LOG_ERROR("Pack or unpack is required");
    LOG_ERROR(options.help());
    return 1;
  }

  if (result.count("unpack")) {
    return Unpack(result);
  } else if (result.count("pack")) {
    return Repack(result);
  }
  return 0;
}
