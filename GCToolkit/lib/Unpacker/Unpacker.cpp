#include "Unpacker/Unpacker.h"

// Util
#include <Logger/Logger.h>

// stl
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>

namespace Unpacking {

namespace {

struct FSTDirectory {
  std::uint32_t fileIndex;
  std::uint32_t parentIndex;
  std::filesystem::path path;
  std::uint32_t lastFile;
};

using DirectoryMapTy = std::unordered_map<std::uint32_t, FSTDirectory>;

FSTDirectory GetParentDir(Parsing::FST &fst, const Parsing::FSTEntry &entry,
                          std::uint32_t fileIndex, DirectoryMapTy &dirMap) {
  // FileOffset contains the parent dir.
  if (entry.fileOffset == 0) {
    FSTDirectory dir{.fileIndex = fileIndex,
                     .parentIndex = 0,
                     .path = fst.GetString(entry.fileNameOffset),
                     .lastFile = entry.fileLen};
    LOG_ERROR("Added dir: {} at index {}", dir.path.string(), fileIndex);
    dirMap.emplace(fileIndex, dir);
    return dir;
  }
  dirMap[entry.fileOffset];

  if (!dirMap.contains(entry.fileOffset)) {
    LOG_ERROR("Parent dir not found at offset {}", entry.fileOffset);
  }
  auto parentEntry = fst.GetEntry(entry.fileOffset - 1);
  auto parentDir = dirMap[entry.fileOffset];
  FSTDirectory dir{.fileIndex = fileIndex,
                   .parentIndex = entry.fileOffset,
                   .path = parentDir.path / fst.GetString(entry.fileNameOffset),
                   .lastFile = entry.fileLen};
  LOG_ERROR("Added dir: {} at index {}", dir.path.string(), fileIndex);
  dirMap.emplace(fileIndex, dir);
  return dir;
}

void WriteAssets(std::filesystem::path basePath, Parsing::GCMFile &gcm,
                 Parsing::FST &fst) {
  auto *buffer = gcm.GetFileBuffer().data();
  auto curPath = basePath;
  DirectoryMapTy directoryMap;
  std::uint32_t fileIndex = 0;
  FSTDirectory currentDir;
  for (const auto &entry : fst) {
    ++fileIndex;
    if (entry.flags & 1) {
      auto fstDir = GetParentDir(fst, entry, fileIndex, directoryMap);
      auto dirPath = basePath / fstDir.path;
      currentDir = fstDir;
      auto res = std::filesystem::create_directory(dirPath);
      curPath = dirPath;
      LOG_TRACE("Created dir: {} index {} lastIndex {}", dirPath.string(),
                fileIndex, fstDir.lastFile);
      if (!res) {
        LOG_ERROR("Failed to create dir: {}", dirPath.string());
      }
      continue;
    }

    // The file index is 1 higher then the lastIndex, if we are at the last file
    // we can already move back to the parent.
    if (fileIndex == currentDir.lastFile) {
      LOG_ERROR("Looking for dir at index {}", currentDir.parentIndex);
      currentDir = directoryMap[currentDir.parentIndex];
      curPath = basePath / currentDir.path;
    }

    auto &fileName = fst.GetString(entry.fileNameOffset);
    auto filePath = curPath / fileName;
    std::ofstream ofstrm(filePath, std::ios::binary);
    auto *filePtr = buffer + entry.fileOffset;
    LOG_TRACE("Creating file: {} index {}", filePath.string(), fileIndex);

    ofstrm.write(reinterpret_cast<char *>(filePtr), entry.fileLen);
  }
}
} // namespace

Unpacker::Unpacker(UnpackerOptions options) : mOptions(options) {
  mGcm.Parse(mOptions.inputISO);
}

bool Unpacker::Unpack() {

  std::filesystem::path out = mOptions.outDir;
  if (!std::filesystem::is_directory(out)) {
    auto res = std::filesystem::create_directory(out);
    LOG_TRACE("Created dir: {}", out.string());
    if (!res) {
      LOG_ERROR("Failed to create dir: {}", out.string());
      return false;
    }
  }

  const std::filesystem::path sys = out / "sys";

  if (!std::filesystem::is_directory(sys)) {
    auto res = std::filesystem::create_directory(sys);
    LOG_TRACE("Created dir: {}", sys.string());
    if (!res) {
      LOG_ERROR("Failed to create dir: {}", sys.string());
      return false;
    }
  }

  std::ofstream bootofstrm(sys / Parsing::BootFileName, std::ios::binary);
  mGcm.WriteBootHeader(bootofstrm);
  bootofstrm.close();

  std::ofstream diskofstrm(sys / Parsing::DiskFileName, std::ios::binary);
  mGcm.WriteDiskHeader(diskofstrm);
  diskofstrm.close();

  std::ofstream applofstrm(sys / Parsing::AppLoaderFileName, std::ios::binary);
  mGcm.WriteAppLoaderHeader(applofstrm);
  applofstrm.close();

  std::ofstream dolofstrm(sys / Parsing::DolFileName, std::ios::binary);
  mGcm.WriteDolFile(dolofstrm);
  dolofstrm.close();

  std::ofstream fstofstrm(sys / Parsing::FSTFileName, std::ios::binary);
  mGcm.WriteFSTFile(fstofstrm);
  fstofstrm.close();

  const std::filesystem::path res = out / "res";

  if (!std::filesystem::is_directory(res)) {
    auto result = std::filesystem::create_directory(res);
    LOG_TRACE("Created dir: {}", res.string());
    if (!result) {
      LOG_ERROR("Failed to create dir: {}", res.string());
      return false;
    }
  }

  Parsing::FST fst;
  auto buffer = mGcm.GetFileBuffer();
  auto FSTOffset = mGcm.GetBootHeader()->FSTOffset;
  fst.Parse(buffer.data() + FSTOffset, mGcm.GetBootHeader()->FSTSize);

  std::ofstream fstdumpstr(sys / "fstDump.txt", std::ios::binary);
  fst.Print(fstdumpstr);
  fst.PrintStrings(fstdumpstr);
  fstdumpstr.close();

  WriteAssets(res, mGcm, fst);

  return true;
}

void Unpacker::Dump() {
  LOG_INFO("Dumping iso to: {}", mOptions.dump);
  std::ofstream dumpstrm(mOptions.dump, std::ios::binary);

  mGcm.PrintBootHeader(dumpstrm);
  mGcm.PrintDiskHeader(dumpstrm);
  mGcm.PrintAppLoaderHeader(dumpstrm);

  dumpstrm.close();
}

} // namespace Unpacking
