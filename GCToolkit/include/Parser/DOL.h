#pragma once

#include <cstdint>
#include <filesystem>
#include <istream>
#include <string>
#include <vector>

namespace Parsing {

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
  DOLFile(const std::string &file);
  bool Parse(const std::string &file);
  bool Parse(std::istream &stream);
  bool ParseHeader(std::istream &input);
  bool ReadData(std::istream &input);

  void PrintHeader(std::ostream &os);
  void PrintData(std::ostream &os, std::size_t begin = 0,
                 std::size_t sizea = 0);
  DOLHeader *GetHeader() { return mHeader; }
  const DOLHeader *GetHeader() const { return mHeader; }
  std::vector<std::uint8_t> &GetData() { return mDataBuffer; }
  const std::vector<std::uint8_t> &GetData() const { return mDataBuffer; }

  const std::string &GetFileName() const { return mFileName; }

private:
  DOLHeader *mHeader;
  std::vector<std::uint8_t> mHeaderBuffer;
  std::vector<std::uint8_t> mDataBuffer;
  std::string mFileName;
};

} // namespace Parsing
