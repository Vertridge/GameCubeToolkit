#include "Parser/GCM.h"

#include "Util/Binary.h"

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace Parsing;

GCMFile::GCMFile(std::string file) { Parse(file); }

bool GCMFile::Parse(std::istream &stream) {
  if (stream.peek() == EOF) {
    std::cerr << "Failed to open: " << mFileName << " " << std::strerror(errno)
              << std::endl;
    return false;
  }
  std::cerr << "Parsing header" << std::endl;
  ParseBootHeader(stream);
  ParseDiskHeader(stream);
  ParseAppLoaderHeader(stream);

  return true;
}

bool GCMFile::Parse(std::string file) {
  mFileName = file;
  std::cout << "Parsing File: " << mFileName << std::endl;
  std::ifstream input(file.c_str(), std::ios::binary);
  if (!input.is_open()) {
    std::cerr << "Failed to open: " << mFileName << " " << std::strerror(errno)
              << std::endl;
    return false;
  }
  return Parse(input);
}

bool GCMFile::ParseBootHeader(std::istream &stream) {
  mBootHeaderBuffer.resize(sizeof(BootHeader));
  stream.read(reinterpret_cast<char *>(mBootHeaderBuffer.data()),
              mBootHeaderBuffer.size());
  mBootHeader = reinterpret_cast<BootHeader *>(mBootHeaderBuffer.data());

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

  if (mBootHeader->DVDMagic != ExpectedDVDMagic) {
    std::cerr << "DVD Magic " << std::hex << mBootHeader->DVDMagic
              << " does not match expected " << std::hex << ExpectedDVDMagic
              << std::endl;
    return false;
  }

  return true;
}

bool GCMFile::ParseDiskHeader(std::istream &stream) {
  mDiskHeaderInfoBuffer.resize(sizeof(DiskHeaderInformation));
  stream.read(reinterpret_cast<char *>(mDiskHeaderInfoBuffer.data()),
              mDiskHeaderInfoBuffer.size());
  mDiskHeaderInfo =
      reinterpret_cast<DiskHeaderInformation *>(mDiskHeaderInfoBuffer.data());

  util::SwapBinary(mDiskHeaderInfo->DebugMonitorSize);
  util::SwapBinary(mDiskHeaderInfo->SimulatedMemorySize);
  util::SwapBinary(mDiskHeaderInfo->ArgumentOffset);
  util::SwapBinary(mDiskHeaderInfo->DebugFlag);
  util::SwapBinary(mDiskHeaderInfo->TrackLocation);
  util::SwapBinary(mDiskHeaderInfo->TrackSize);
  util::SwapBinary(mDiskHeaderInfo->CountryCode);

  return true;
}

bool GCMFile::ParseAppLoaderHeader(std::istream &stream) {
  mAppLoaderHeaderBuffer.resize(sizeof(AppLoaderHeader));
  stream.read(reinterpret_cast<char *>(mAppLoaderHeaderBuffer.data()),
              mAppLoaderHeaderBuffer.size());
  mAppLoaderHeader =
      reinterpret_cast<AppLoaderHeader *>(mAppLoaderHeaderBuffer.data());

  util::SwapBinary(mAppLoaderHeader->AppLoaderEntryPoint);
  util::SwapBinary(mAppLoaderHeader->AppLoaderSize);
  util::SwapBinary(mAppLoaderHeader->TrailerSize);

  return true;
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

void GCMFile::PrintDiskHeader(std::ostream &os) {
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

  // for(auto i = 0; i < DiskHeaderInfoDataSize; ++i) {
  //   os << mDiskHeaderInfo->Data[i] << " ";
  // }
  // os << "\n";
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
