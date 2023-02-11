#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace Patcher::Parsing {

constexpr std::size_t DOLNumberOfTextSections = 7;
constexpr std::size_t DOLNumberOfDataSections = 11;
constexpr std::size_t DOLPaddingSize = 7;

struct DOLHeader {
  std::uint32_t TextSectionOffsets[DOLNumberOfTextSections];
  std::uint32_t DataSectionOffsets[DOLNumberOfDataSections];
  std::uint32_t TextSectionAddresses[DOLNumberOfTextSections];
  std::uint32_t DataSectionAddresses[DOLNumberOfDataSections];
  std::uint32_t TextSectionSizes[DOLNumberOfTextSections];
  std::uint32_t DataSectionSizes[DOLNumberOfDataSections];
  std::uint32_t BSSAddress;
  std::uint32_t BSSSize;
  std::uint32_t EntryPointAddress;
  std::uint32_t Padding[DOLPaddingSize];
};

class DOLFile {
public:
  DOLFile() = default;
  DOLFile(std::filesystem::path file);
  bool Parse(std::filesystem::path file);

  void PrintHeader();
  void PrintData(std::size_t begin = 0, std::size_t sizea = 0);

private:
  bool ParseHeader(std::ifstream &input);

  bool ReadData(std::ifstream &input);

  DOLHeader *mHeader;
  std::vector<char> mHeaderBuffer;
  std::vector<char> mDataBuffer;
  std::string mFileName;
};

} // namespace Patcher::Parsing
