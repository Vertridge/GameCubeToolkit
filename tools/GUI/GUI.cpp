#include <iostream>

#include "GUIProgram.h"

// Platform
#include <Platform/FileSelector.h>

// Vendor
#include "cxxopts.hpp"

// stl
#include <filesystem>

struct GUIOptions {
  std::string input = "";
  std::string dis = "disassembly.txt";
  std::string dump = "dump.txt";
};

GUIOptions ParseCMDOptions(int argc, char *argv[]) {

  cxxopts::Options options("GCToolkit", "GCToolkit GUI");

  // clang-format off
  options.add_options()
      ("dump,dump-file", "Dump file", cxxopts::value<std::string>())
      ("dis,disassembly", "Disassenbly file", cxxopts::value<std::string>())
      ("h,help", "Print usage")
      ("iso", "Input", cxxopts::value<std::string>());
  // clang-format on

  options.parse_positional({"iso"});

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }

  GUIOptions guiOptions;

  if (result.count("dis")) {
    guiOptions.dis = result["dis"].as<std::string>();
  }

  if (result.count("dump")) {
    guiOptions.dump = result["dump"].as<std::string>();
  }

  return guiOptions;
}

int main(int argc, char *argv[]) {
  std::cout << "GUI Started" << std::endl;

  auto options = ParseCMDOptions(argc, argv);

  if (options.input == "") {
    std::cout << "Select input file" << std::endl;
    options.input = Platform::FileSelector::SelectFile();
  }
  if (!std::filesystem::is_regular_file(options.input)) {
    std::cerr << "Invalid file '" << options.input << "' selected."
              << std::endl;
    exit(1);
  }

  std::cout << "Opening iso file: '" << options.input << "'." << std::endl;

  GUI::GUIProgram program({options.input, options.dis, options.dump});
  program.Init();
  program.Start();

  return 0;
}
