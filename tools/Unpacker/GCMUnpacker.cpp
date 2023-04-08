#include <filesystem>
#include <fstream>
#include <iostream>

#include "Parser/FST.h"
#include "Parser/GCM.h"

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

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Pass File to Unpack" << std::endl;
    return 1;
  }

  std::cout << "GCM Unpacker: " << argv[1] << std::endl;

  Parsing::GCMFile gcm;

  if (!gcm.Parse(argv[1])) {
    std::cerr << "Failed to open: " << argv[1] << std::endl;
    return 1;
  }

  std::string output = "gcmDump.txt";
  if (argc >= 3) {
    output = argv[2];
  }

  std::ofstream ofstrm(output, std::ios::binary);

  gcm.PrintBootHeader(ofstrm);
  gcm.PrintDiskHeader(ofstrm);
  gcm.PrintAppLoaderHeader(ofstrm);

  ofstrm.close();

  std::ofstream bootofstrm(Parsing::BootFileName, std::ios::binary);
  gcm.WriteBootHeader(bootofstrm);
  bootofstrm.close();

  std::ofstream diskofstrm(Parsing::DiskFileName, std::ios::binary);
  gcm.WriteDiskHeader(diskofstrm);
  diskofstrm.close();

  std::ofstream applofstrm(Parsing::AppLoaderFileName, std::ios::binary);
  gcm.WriteAppLoaderHeader(applofstrm);
  applofstrm.close();

  std::ofstream dolofstrm(Parsing::DolFileName, std::ios::binary);
  gcm.WriteDolFile(dolofstrm);
  dolofstrm.close();

  std::ofstream fstofstrm(Parsing::FSTFileName, std::ios::binary);
  gcm.WriteFSTFile(fstofstrm);
  fstofstrm.close();

  Parsing::FST fst;
  auto buffer = gcm.GetFileBuffer();
  auto FSTOffset = gcm.GetBootHeader()->FSTOffset;
  fst.Parse(buffer.data() + FSTOffset, gcm.GetBootHeader()->FSTSize);

  std::ofstream fstdumpstr("fstDump.txt", std::ios::binary);
  fst.Print(fstdumpstr);
  fst.PrintStrings(fstdumpstr);
  fstdumpstr.close();

  WriteAssets("/home/ruben/Projects/c++/Gamecube/GameCubeToolkit/build", gcm,
              fst);
}
