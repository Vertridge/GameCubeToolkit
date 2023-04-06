#pragma once

#include <cstdint>
#include <istream>
#include <vector>

#include "Util/MemoryMap.h"

namespace Parsing {

constexpr auto BootFileName = "boot.bin";
constexpr auto DiskFileName = "bi2.bin";
constexpr auto AppLoaderFileName = "apploader.bin";

constexpr auto ExpectedDVDMagic = 0xc2339f3d;
constexpr auto GameNameSize = 0x03e0;
constexpr auto Padding1Size = 18;
constexpr auto Padding2Size = 24;
constexpr auto Padding3Size = 4;
// The total size is 0x2000 but we subtract 0x20 the size of
// DiskHeaderInformation
constexpr auto DiskHeaderInfoDataSize = 0x2000 - 0x20;

constexpr auto DateStrLength = 0x0A;
constexpr auto AppLoaderPaddingSize = 0x5;

// Every GCM file starts with a header. The header is called the boot.bin file.
struct __attribute__((packed)) BootHeader {
  // Game Code
  std::uint8_t ConsoleID;
  std::uint16_t GameCode;
  std::uint8_t CountryCode;

  std::uint16_t MakerCode;
  std::uint8_t DiskID;
  std::uint8_t Version;
  std::uint8_t AudioStreaming;   // Prob useless
  std::uint8_t StreamBufferSize; // Prob useless

  std::uint8_t Padding[Padding1Size];

  std::uint32_t DVDMagic; // 0xc2339f3d
  std::uint8_t GameName[GameNameSize];

  std::uint32_t DebugMonitorOffset; // dh.bin
  std::uint32_t DebugMonitorLoadAddress;

  std::uint8_t Padding2[Padding2Size];

  std::uint32_t BootFileOffset; // Offset of main executable DOL.
  std::uint32_t FSTOffset;      // fst.bin offset.
  std::uint32_t FSTSize;        // fst.bin size.
  std::uint32_t FSTMaxSize;     // fst.bin max size == fst.bin size.

  std::uint32_t UserPosition; // ?
  std::uint32_t UserLength;   // ?
  std::uint32_t Unknown;      // ?
  std::uint8_t Padding3[Padding3Size];
};

enum class RegionCode { Japan = 0, USA = 1, PAL = 2 };

// Header of the disk bi2.bin.
struct __attribute__((packed)) DiskHeaderInformation {
  std::uint32_t DebugMonitorSize;
  std::uint32_t SimulatedMemorySize;
  std::uint32_t ArgumentOffset;
  std::uint32_t DebugFlag;
  std::uint32_t TrackLocation;
  std::uint32_t TrackSize;
  std::uint32_t CountryCode;
  std::uint8_t Padding[Padding3Size];
  std::uint8_t Data[DiskHeaderInfoDataSize];
};

// appldr.bin
struct __attribute__((packed)) AppLoaderHeader {
  std::uint8_t DateStr[DateStrLength];
  std::uint8_t Padding[AppLoaderPaddingSize];
  std::uint32_t AppLoaderEntryPoint;
  std::uint32_t AppLoaderSize; // Normally 0x2000
  std::uint32_t TrailerSize;
};

class GCMFile {
public:
  GCMFile() = default;
  GCMFile(std::string file);
  ~GCMFile();

  bool Parse(std::string file);
  bool Parse(std::istream &stream);

  bool ParseBootHeader();
  void PrintBootHeader(std::ostream &os);
  void WriteBootHeader(std::ostream &os);
  bool ParseDiskHeader();
  void PrintDiskHeader(std::ostream &os, bool printData = false);
  void WriteDiskHeader(std::ostream &os);
  bool ParseAppLoaderHeader();
  void PrintAppLoaderHeader(std::ostream &os);
  void WriteAppLoaderHeader(std::ostream &os);

  std::string_view GetFileName() { return mFileName; }

private:
  BootHeader *mBootHeader;
  DiskHeaderInformation *mDiskHeaderInfo;
  AppLoaderHeader *mAppLoaderHeader;

  std::vector<std::uint8_t> mFileBuffer;

  std::string mFileName;

  util::MmapMem mMem;
};

} // namespace Parsing
