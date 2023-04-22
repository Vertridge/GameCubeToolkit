#include <filesystem>
#include <fstream>
#include <iostream>

#include "Parser/FST.h"
#include "Parser/GCM.h"

// Vendor
#include "cxxopts.hpp"

std::filesystem::path GetParentDir(Parsing::FST &fst,
                                   const Parsing::FSTEntry &entry) {
  // FileOffset contains the parent dir.
  if (entry.fileOffset == 0) {
    return fst.GetString(entry.fileNameOffset);
  }
  auto parent = fst.GetEntry(entry.fileOffset - 1);
  return GetParentDir(fst, parent) / fst.GetString(entry.fileNameOffset);
}

void WriteAssets(std::filesystem::path basePath, Parsing::GCMFile &gcm,
                 Parsing::FST &fst) {
  auto *buffer = gcm.GetFileBuffer().data();
  auto curPath = basePath;
  for (const auto &entry : fst) {
    if (entry.flags & 1) {
      auto dirName = GetParentDir(fst, entry);

      auto dirPath = basePath / dirName;

      auto res = std::filesystem::create_directory(dirPath);
      curPath = dirPath;
      std::cout << "Created dir: " << dirPath.string() << std::endl;
      if (!res) {
        std::cerr << "Failed to create dir: " << dirPath.string() << std::endl;
      }
      continue;
    }
    auto &fileName = fst.GetString(entry.fileNameOffset);
    auto filePath = curPath / fileName;
    std::ofstream ofstrm(filePath, std::ios::binary);
    auto *filePtr = buffer + entry.fileOffset;
    std::cout << "Creating file: " << filePath.string() << std::endl;

    ofstrm.write(reinterpret_cast<char *>(filePtr), entry.fileLen);
  }
}

int Unpack(cxxopts::ParseResult &result) {
  if (!result.count("gcm")) {
    std::cout << "gcm is required when unpacking see help" << std::endl;
    return 1;
  }

  auto input = result["gcm"].as<std::string>();

  std::cout << "GCM Unpacker: " << input << std::endl;

  Parsing::GCMFile gcm;

  if (!gcm.Parse(input)) {
    std::cerr << "Failed to open: " << input << std::endl;
    return 1;
  }

  std::string dump = "gcmDump.txt";
  if (result.count("dump")) {
    dump = result["dump"].as<std::string>();
  }

  std::ofstream dumpstrm(dump, std::ios::binary);

  gcm.PrintBootHeader(dumpstrm);
  gcm.PrintDiskHeader(dumpstrm);
  gcm.PrintAppLoaderHeader(dumpstrm);

  dumpstrm.close();

  std::filesystem::path out = "out";
  if (result.count("output")) {
    out = result["output"].as<std::string>();
  }

  if (!std::filesystem::is_directory(out)) {
    auto res = std::filesystem::create_directory(out);
    std::cout << "Created dir: " << out.string() << std::endl;
    if (!res) {
      std::cerr << "Failed to create dir: " << out.string() << std::endl;
    }
  }

  const std::filesystem::path sys = out / "sys";

  if (!std::filesystem::is_directory(sys)) {
    auto res = std::filesystem::create_directory(sys);
    std::cout << "Created dir: " << sys.string() << std::endl;
    if (!res) {
      std::cerr << "Failed to create dir: " << sys.string() << std::endl;
    }
  }

  std::ofstream bootofstrm(sys / Parsing::BootFileName, std::ios::binary);
  gcm.WriteBootHeader(bootofstrm);
  bootofstrm.close();

  std::ofstream diskofstrm(sys / Parsing::DiskFileName, std::ios::binary);
  gcm.WriteDiskHeader(diskofstrm);
  diskofstrm.close();

  std::ofstream applofstrm(sys / Parsing::AppLoaderFileName, std::ios::binary);
  gcm.WriteAppLoaderHeader(applofstrm);
  applofstrm.close();

  std::ofstream dolofstrm(sys / Parsing::DolFileName, std::ios::binary);
  gcm.WriteDolFile(dolofstrm);
  dolofstrm.close();

  std::ofstream fstofstrm(sys / Parsing::FSTFileName, std::ios::binary);
  gcm.WriteFSTFile(fstofstrm);
  fstofstrm.close();

  const std::filesystem::path res = out / "res";

  if (!std::filesystem::is_directory(res)) {
    auto result = std::filesystem::create_directory(res);
    std::cout << "Created dir: " << res.string() << std::endl;
    if (!result) {
      std::cerr << "Failed to create dir: " << res.string() << std::endl;
    }
  }

  Parsing::FST fst;
  auto buffer = gcm.GetFileBuffer();
  auto FSTOffset = gcm.GetBootHeader()->FSTOffset;
  fst.Parse(buffer.data() + FSTOffset, gcm.GetBootHeader()->FSTSize);

  std::ofstream fstdumpstr(sys / "fstDump.txt", std::ios::binary);
  fst.Print(fstdumpstr);
  fst.PrintStrings(fstdumpstr);
  fstdumpstr.close();

  WriteAssets(res, gcm, fst);
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
