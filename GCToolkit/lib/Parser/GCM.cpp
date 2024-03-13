#include "Parser/GCM.h"

#include "Binary.h"

// Util
#include <Logger/Logger.h>

// stl
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ostream>

namespace Parsing {

namespace {

constexpr std::size_t GetBooHeaderOffset() { return 0; }
constexpr std::size_t GetDiskHeaderOffset() { return sizeof(BootHeader); }
constexpr std::size_t GetAppLoaderHeaderOffset() {
  return sizeof(BootHeader) + sizeof(DiskHeaderInformation);
}

} // namespace

GCMFile::GCMFile(std::string file) { Parse(file); }

GCMFile::~GCMFile() { util::MunMap(mMem); }

bool GCMFile::Parse(std::istream &stream) {
  if (stream.peek() == EOF) {
    LOG_ERROR("Failed to open: '{}' - {}", mFileName, std::strerror(errno));
    return false;
  }
  LOG_TRACE("Parsing header");

  std::istream_iterator<std::uint8_t> start(stream);
  mFileBuffer = std::vector<std::uint8_t>(start, {});

  ParseBootHeader();
  ParseDiskHeader();
  ParseAppLoaderHeader();

  return true;
}

bool GCMFile::Parse(std::string file) {
  mFileName = file;
  LOG_INFO("Parsing File: {}", mFileName);

  auto memMap = util::MmapFile(file);

  mFileBuffer.resize(memMap.Len);
  mFileBuffer.assign(reinterpret_cast<std::uint8_t *>(memMap.Addr),
                     reinterpret_cast<std::uint8_t *>(memMap.Addr) +
                         memMap.Len);

  ParseBootHeader();
  ParseDiskHeader();
  ParseAppLoaderHeader();

  return true;
}

bool GCMFile::Write(std::ostream &os) {

  LOG_INFO("Writing GCM");

  os.write(reinterpret_cast<char *>(mFileBuffer.data()), mFileBuffer.size());

  return true;
}

bool GCMFile::ParseBootHeader(bool binswap) {
  const auto offset = GetBooHeaderOffset();
  assert(mFileBuffer.size() >= sizeof(BootHeader));
  mBootHeader = reinterpret_cast<BootHeader *>(mFileBuffer.data() + offset);

  if (binswap) {
    // Swap endiness.
    util::SwapBinary(mBootHeader->GameCode);
    util::SwapBinary(mBootHeader->MakerCode);

    util::SwapBinary(mBootHeader->DVDMagic);

    util::SwapBinary(mBootHeader->DebugMonitorOffset);
    util::SwapBinary(mBootHeader->DebugMonitorLoadAddress);

    util::SwapBinary(mBootHeader->BootFileOffset);
    util::SwapBinary(mBootHeader->FSTOffset);
    util::SwapBinary(mBootHeader->FSTSize);
    util::SwapBinary(mBootHeader->FSTMaxSize);

    util::SwapBinary(mBootHeader->UserPosition);
    util::SwapBinary(mBootHeader->UserLength);
    util::SwapBinary(mBootHeader->Unknown);
  }

  if (mBootHeader->DVDMagic != ExpectedDVDMagic) {
    LOG_ERROR("DVD Magic {0:x} does not match expected {0:x}",
              mBootHeader->DVDMagic, ExpectedDVDMagic);
    return false;
  }

  return true;
}

bool GCMFile::ParseDiskHeader(bool binswap) {
  const std::size_t offset = GetDiskHeaderOffset();
  assert(mFileBuffer.size() >= offset + sizeof(DiskHeaderInformation));

  mDiskHeaderInfo =
      reinterpret_cast<DiskHeaderInformation *>(mFileBuffer.data() + offset);

  if (!binswap) {
    return true;
  }

  util::SwapBinary(mDiskHeaderInfo->DebugMonitorSize);
  util::SwapBinary(mDiskHeaderInfo->SimulatedMemorySize);
  util::SwapBinary(mDiskHeaderInfo->ArgumentOffset);
  util::SwapBinary(mDiskHeaderInfo->DebugFlag);
  util::SwapBinary(mDiskHeaderInfo->TrackLocation);
  util::SwapBinary(mDiskHeaderInfo->TrackSize);
  util::SwapBinary(mDiskHeaderInfo->CountryCode);

  return true;
}

bool GCMFile::ParseAppLoaderHeader(bool binswap) {
  const std::size_t offset = GetAppLoaderHeaderOffset();
  assert(mFileBuffer.size() >= offset + sizeof(AppLoaderHeader));

  mAppLoaderHeader =
      reinterpret_cast<AppLoaderHeader *>(mFileBuffer.data() + offset);

  if (!binswap) {
    return true;
  }

  util::SwapBinary(mAppLoaderHeader->AppLoaderEntryPoint);
  util::SwapBinary(mAppLoaderHeader->AppLoaderSize);
  util::SwapBinary(mAppLoaderHeader->TrailerSize);

  return true;
}

void GCMFile::WriteBootHeader(std::ostream &os) {
  os.write(reinterpret_cast<char *>(mBootHeader), sizeof(BootHeader));
}

void GCMFile::WriteDiskHeader(std::ostream &os) {
  os.write(reinterpret_cast<char *>(mDiskHeaderInfo),
           sizeof(DiskHeaderInformation));
}

void GCMFile::WriteDolFile(std::ostream &os) {
  if (mBootHeader == nullptr) {
    LOG_ERROR("Cannot write dol file, boot header has to be parsed");

    return;
  }
  auto dolOffset = mBootHeader->BootFileOffset;
  // Dol file does not have an end, but ends when the FST file starts.
  auto dolLen = mBootHeader->FSTOffset - dolOffset;
  auto dolAddr = mFileBuffer.data() + dolOffset;
  os.write(reinterpret_cast<char *>(dolAddr), dolLen);
}

void GCMFile::WriteFSTFile(std::ostream &os) {
  if (mBootHeader == nullptr) {
    LOG_ERROR("Cannot write FST file, boot header has to be parsed");

    return;
  }
  auto fst = mFileBuffer.data() + mBootHeader->FSTOffset;
  os.write(reinterpret_cast<char *>(fst), mBootHeader->FSTSize);
}

void GCMFile::WriteAppLoaderHeader(std::ostream &os) {
  os.write(reinterpret_cast<char *>(mAppLoaderHeader), sizeof(AppLoaderHeader));
}

void GCMFile::ReadBootHeader(std::istream &is) {
  const std::size_t offset = GetBooHeaderOffset();
  assert(mFileBuffer.capacity() == offset &&
         "Buffer does not have correct size before resizing");
  mFileBuffer.reserve(sizeof(BootHeader));
  is.read(reinterpret_cast<char *>(mFileBuffer.data() + offset),
          sizeof(BootHeader));

  ParseBootHeader(false);
}

void GCMFile::ReadDiskHeader(std::istream &is) {
  const std::size_t offset = GetDiskHeaderOffset();
  assert(mFileBuffer.capacity() == offset &&
         "Buffer does not have correct size before resizing");

  mFileBuffer.reserve(sizeof(DiskHeaderInformation));
  is.read(reinterpret_cast<char *>(mFileBuffer.data() + offset),
          sizeof(DiskHeaderInformation));

  ParseDiskHeader(false);
}

void GCMFile::ReadAppLoaderHeader(std::istream &is) {
  const std::size_t offset = GetAppLoaderHeaderOffset();
  assert(mFileBuffer.capacity() == offset &&
         "Buffer does not have correct size before resizing");

  mFileBuffer.reserve(sizeof(DiskHeaderInformation));
  is.read(reinterpret_cast<char *>(mFileBuffer.data() + offset),
          sizeof(DiskHeaderInformation));

  ParseAppLoaderHeader(false);
}

void GCMFile::ReadDolFile(std::istream &is, std::size_t size) {
  if (mBootHeader == nullptr) {
    LOG_ERROR("Cannot write dol file, boot header has to be parsed");

    return;
  }

  auto dolOffset = mFileBuffer.capacity();
  mFileBuffer.reserve(size);
  is.read(reinterpret_cast<char *>(mFileBuffer.data() + dolOffset), size);
  mBootHeader->BootFileOffset = dolOffset;
}

void GCMFile::ReadFSTFile(std::istream &is, std::size_t size) {
  if (mBootHeader == nullptr) {
    LOG_ERROR("Cannot write fst file, boot header has to be parsed");

    return;
  }
  auto fstOffset = mFileBuffer.capacity();
  mFileBuffer.reserve(size);
  is.read(reinterpret_cast<char *>(mFileBuffer.data() + fstOffset), size);
  mBootHeader->FSTOffset = fstOffset;
  mBootHeader->FSTSize = size;
}

void GCMFile::PrintBootHeader(std::ostream &os) {
  os << "boot.bin 0x" << std::hex << sizeof(BootHeader) << " :\n";

  // clang-format off
  os << "Console ID: 0x" << std::setw(2) << std::setfill('0') 
     << std::hex << (int)(mBootHeader->ConsoleID & 0xff) << "\n";

  os << "Game code: 0x" << std::hex << mBootHeader->GameCode << "\n";
  
  os << "Country code: 0x" << std::setw(2) << std::setfill('0') 
     << std::hex << (int)(mBootHeader->CountryCode & 0xff) << "\n";

  os << "Maker code: 0x" << std::hex << mBootHeader->MakerCode << "\n";

  os << "Disk ID: 0x" << std::setw(2) << std::setfill('0') 
     << std::hex << (int)(mBootHeader->DiskID & 0xff) << "\n";

  os << "Version: 0x" << std::setw(2) << std::setfill('0') 
     << std::hex << (int)(mBootHeader->Version & 0xff) << "\n";

  os << "Audio Streaming: 0x" << std::setw(2) << std::setfill('0') 
     << std::hex << (int)(mBootHeader->AudioStreaming & 0xff) << "\n";

  os << "Stream Buffer size: 0x" << std::setw(2) << std::setfill('0') 
     << std::hex << (int)(mBootHeader->StreamBufferSize & 0xff) << "\n";

  os << "Padding: \n";
  for(auto i = 0; i < Padding1Size; ++i) {
    os << "\t0x"  << std::setw(2) << std::setfill('0') << std::hex 
       << (int)(mBootHeader->Padding[i] & 0xff) << "\n";
  }

  os << "DVD Magic: 0x" << std::hex << mBootHeader->DVDMagic << "\n";

  os << "Game name: ";
  for(auto i = 0; i < GameNameSize; ++i) {
    os << mBootHeader->GameName[i];
  }
  os << "\n";

  os << "Debug Monitor Offset: 0x" << std::hex << mBootHeader->DebugMonitorOffset << "\n";
  os << "Debug Monitor Address: 0x" << std::hex << mBootHeader->DebugMonitorLoadAddress << "\n";
  
  os << "Padding: \n";
  for(auto i = 0; i < Padding2Size; ++i) {
    os << "\t0x"  << std::setw(2) << std::setfill('0') << std::hex 
       << (int)(mBootHeader->Padding2[i] & 0xff) << "\n";
  }

  os << "Boot File Offset: 0x" << std::hex << mBootHeader->BootFileOffset << "\n";
  os << "FST File Offset: 0x" << std::hex << mBootHeader->FSTOffset << "\n";
  os << "FST File Size: 0x" << std::hex << mBootHeader->FSTSize << "\n";
  os << "FST File Max Size: 0x" << std::hex << mBootHeader->FSTMaxSize << "\n";

  os << "User Position: 0x" << std::hex << mBootHeader->UserPosition << "\n";
  os << "User Length: 0x" << std::hex << mBootHeader->UserLength << "\n";
  os << "Unknown: 0x" << std::hex << mBootHeader->Unknown << "\n";

  os << "Padding: \n";
  for(auto i = 0; i < Padding3Size; ++i) {
    os << "\t0x"  << std::setw(2) << std::setfill('0') << std::hex 
       << (int)(mBootHeader->Padding3[i] & 0xff) << "\n";
  }
  os << "\n";
  // clang-format on
}

void GCMFile::PrintDiskHeader(std::ostream &os, bool printData) {
  os << "bi2.bin 0x" << std::hex << sizeof(DiskHeaderInformation) << " :\n";

  // clang-format off
  os << "Debug Monitor Size: 0x" << std::hex << mDiskHeaderInfo->DebugMonitorSize << "\n";
  os << "Simulated Memory Size: 0x" << std::hex << mDiskHeaderInfo->SimulatedMemorySize << "\n";
  os << "Argument offset: 0x" << std::hex << mDiskHeaderInfo->ArgumentOffset << "\n";
  os << "Debug Flag: 0x" << std::hex << mDiskHeaderInfo->DebugFlag << "\n";
  os << "Track Location: 0x" << std::hex << mDiskHeaderInfo->TrackLocation << "\n";
  os << "Track Size: 0x" << std::hex << mDiskHeaderInfo->TrackSize << "\n";
  os << "Country Code: 0x" << std::hex << mDiskHeaderInfo->CountryCode << "\n";

  os << "Uknown: \n";
  for(auto i = 0; i < Padding3Size; ++i) {
    os << "\t0x"  << std::setw(2) << std::setfill('0') << std::hex 
       << (int)(mDiskHeaderInfo->Padding[i] & 0xff) << "\n";
  }
  os << "\n";

  if(printData) {
    for(auto i = 0; i < DiskHeaderInfoDataSize; ++i) {
      os << mDiskHeaderInfo->Data[i];
    }
  }
  os << "\n";
  // clang-format on
}

void GCMFile::PrintAppLoaderHeader(std::ostream &os) {
  os << "appldr.bin 0x" << std::hex << sizeof(AppLoaderHeader) << "\n";

  // clang-format off
  os << "Date: ";
  for(auto i = 0; i < DateStrLength; ++i) {
    os << mAppLoaderHeader->DateStr[i];
  }
  os << "\n";

  os << "Padding: \n";
  for(auto i = 0; i < AppLoaderPaddingSize; ++i) {
    os << "\t0x"  << std::setw(2) << std::setfill('0') << std::hex 
       << (int)(mAppLoaderHeader->Padding[i] & 0xff) << "\n";
  }
  os << "AppLoader EntryPoint: 0x" << std::hex << mAppLoaderHeader->AppLoaderEntryPoint << "\n";
  os << "AppLoader Size: 0x" << std::hex << mAppLoaderHeader->AppLoaderSize << "\n";
  os << "Trailer Size: 0x" << std::hex << mAppLoaderHeader->TrailerSize << "\n";
  // clang-format on
}

} // namespace Parsing
