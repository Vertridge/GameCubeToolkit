#include "Unpacker/Unpacker.h"

// Util
#include <Logger/Logger.h>

// stl
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

namespace Unpacking {

namespace {
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
      LOG_TRACE("Created dir: {}", dirPath.string());
      if (!res) {
        LOG_ERROR("Failed to create dir: {}", dirPath.string());
      }
      continue;
    }
    auto &fileName = fst.GetString(entry.fileNameOffset);
    auto filePath = curPath / fileName;
    std::ofstream ofstrm(filePath, std::ios::binary);
    auto *filePtr = buffer + entry.fileOffset;
    LOG_TRACE("Creating file: {}", filePath.string());

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
