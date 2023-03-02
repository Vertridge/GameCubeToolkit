#include "catch.hpp"

#include "Parser/DOL.h"

#include "Util/MemoryStream.h"

#include <iostream>
#include <sstream>

using namespace Parsing;

TEST_CASE("DOL Header Parsing") {

  // clang-format off
  std::vector<std::uint8_t> DOLHeaderData = {
      0x00, 0x00, 0x01, 0x00, // TextSection offset 1
      0x00, 0x00, 0x25, 0x40, // TextSection offset 2
      0x00, 0x00, 0x00, 0x00, // TextSection offset 3
      0x00, 0x00, 0x00, 0x00, // TextSection offset 4
      0x00, 0x00, 0x00, 0x00, // TextSection offset 5
      0x00, 0x00, 0x00, 0x00, // TextSection offset 6
      0x00, 0x00, 0x00, 0x00, // TextSection offset 7
    
      0x00, 0x36, 0x82, 0x20, // DataSection offset 1
      0x00, 0x36, 0x82, 0x80, // DataSection offset 2
      0x00, 0x36, 0x82, 0xe0, // DataSection offset 3
      0x00, 0x36, 0x86, 0xa0, // DataSection offset 4
      0x00, 0x36, 0x86, 0xc0, // DataSection offset 5
      0x00, 0x3a, 0x09, 0xe0, // DataSection offset 6
      0x00, 0x3d, 0xe0, 0xc0, // DataSection offset 7
      0x00, 0x3d, 0xed, 0xc0, // DataSection offset 8
      0x00, 0x00, 0x00, 0x00, // DataSection offset 9
      0x00, 0x00, 0x00, 0x00, // DataSection offset 10
      0x00, 0x00, 0x00, 0x00, // DataSection offset 11

      0x80, 0x00, 0x31, 0x00, // TextSection address 1
      0x80, 0x00, 0x56, 0x00, // TextSection address 2
      0x00, 0x00, 0x00, 0x00, // TextSection address 3
      0x00, 0x00, 0x00, 0x00, // TextSection address 4
      0x00, 0x00, 0x00, 0x00, // TextSection address 5
      0x00, 0x00, 0x00, 0x00, // TextSection address 6
      0x00, 0x00, 0x00, 0x00, // TextSection address 7

      0x80, 0x00, 0x55, 0x40, // DataSection address 1
      0x80, 0x00, 0x55, 0xa0, // DataSection address 2
      0x80, 0x36, 0xb2, 0xe0, // DataSection address 3
      0x80, 0x36, 0xb6, 0xa0, // DataSection address 4
      0x80, 0x36, 0xb6, 0xc0, // DataSection address 5
      0x80, 0x3a, 0x39, 0xe0, // DataSection address 6
      0x80, 0x40, 0x39, 0x60, // DataSection address 7
      0x80, 0x40, 0x62, 0x80, // DataSection address 8
      0x00, 0x00, 0x00, 0x00, // DataSection address 9
      0x00, 0x00, 0x00, 0x00, // DataSection address 10
      0x00, 0x00, 0x00, 0x00, // DataSection address 11

      0x00, 0x00, 0x24, 0x40, // TextSection size 1
      0x00, 0x36, 0x5c, 0xe0, // TextSection size 2
      0x00, 0x00, 0x00, 0x00, // TextSection size 3
      0x00, 0x00, 0x00, 0x00, // TextSection size 4
      0x00, 0x00, 0x00, 0x00, // TextSection size 5
      0x00, 0x00, 0x00, 0x00, // TextSection size 6
      0x00, 0x00, 0x00, 0x00, // TextSection size 7

      0x00, 0x00, 0x00, 0x60, // DataSection size 1
      0x00, 0x00, 0x00, 0x60, // DataSection size 2
      0x00, 0x00, 0x03, 0xc0, // DataSection size 3
      0x00, 0x00, 0x00, 0x20, // DataSection size 4
      0x00, 0x03, 0x83, 0x20, // DataSection size 5
      0x00, 0x03, 0xd6, 0xe0, // DataSection size 6
      0x00, 0x00, 0x0d, 0x00, // DataSection size 7
      0x00, 0x00, 0x8a, 0xe0, // DataSection size 8
      0x00, 0x00, 0x00, 0x00, // DataSection size 9
      0x00, 0x00, 0x00, 0x00, // DataSection size 10
      0x00, 0x00, 0x00, 0x00, // DataSection size 11

      0x80, 0x3e, 0x10, 0xc0, // BSS Address
      0x00, 0x02, 0x51, 0xb8, // BSS Size
      0x80, 0x00, 0x52, 0x2c, // Entry Point

      0x00, 0x00, 0x00, 0x00, // Padding 1
      0x00, 0x00, 0x00, 0x00, // Padding 2
      0x00, 0x00, 0x00, 0x00, // Padding 3
      0x00, 0x00, 0x00, 0x00, // Padding 4
      0x00, 0x00, 0x00, 0x00, // Padding 5
      0x00, 0x00, 0x00, 0x00, // Padding 6
      0x00, 0x00, 0x00, 0x00, // Padding 7
  };

  // clang-format on

  std::uint32_t TextSectionOffsets[Parsing::DOLNumberOfTextSections] = {
      256, 9536, 0, 0, 0, 0, 0};
  std::uint32_t DataSectionOffsets[Parsing::DOLNumberOfDataSections] = {
      3572256, 3572352, 3572448, 3573408, 3573440, 3803616,
      4055232, 4058560, 0,       0,       0};
  std::uint32_t TextSectionAddresses[Parsing::DOLNumberOfTextSections] = {
      2147496192, 2147505664, 0, 0, 0, 0, 0};
  std::uint32_t DataSectionAddresses[Parsing::DOLNumberOfDataSections] = {
      2147505472, 2147505568, 2151068384, 2151069344, 2151069376, 2151299552,
      2151692640, 2151703168, 0,          0,          0};
  std::uint32_t TextSectionSizes[Parsing::DOLNumberOfTextSections] = {
      9280, 3562720, 0, 0, 0, 0, 0};
  std::uint32_t DataSectionSizes[Parsing::DOLNumberOfDataSections] = {
      96, 96, 960, 32, 230176, 251616, 3328, 35552, 0, 0, 0};
  std::uint32_t BSSAddress = 2151551168;
  std::uint32_t BSSSize = 151992;
  std::uint32_t EntryPointAddress = 2147504684;
  std::uint32_t Padding[DOLPaddingSize] = {0, 0, 0, 0, 0, 0, 0};

  util::MemoryStream memstream(DOLHeaderData.data(), DOLHeaderData.size());

  DOLFile dol;
  dol.ParseHeader(memstream);

  std::ostringstream dolHeaderString;
  dol.PrintHeader(dolHeaderString);
  auto header = dol.GetHeader();

  std::cout << dolHeaderString.str() << std::endl;

  for (std::size_t i = 0; i < Parsing::DOLNumberOfTextSections; ++i) {
    REQUIRE(header->TextSectionOffsets[i] == TextSectionOffsets[i]);
    REQUIRE(header->TextSectionAddresses[i] == TextSectionAddresses[i]);
    REQUIRE(header->TextSectionSizes[i] == TextSectionSizes[i]);
  }

  for (std::size_t i = 0; i < Parsing::DOLNumberOfDataSections; ++i) {
    REQUIRE(header->DataSectionOffsets[i] == DataSectionOffsets[i]);
    REQUIRE(header->DataSectionAddresses[i] == DataSectionAddresses[i]);
    REQUIRE(header->DataSectionSizes[i] == DataSectionSizes[i]);
  }

  REQUIRE(header->BSSAddress == BSSAddress);
  REQUIRE(header->BSSSize == BSSSize);
  REQUIRE(header->EntryPointAddress == EntryPointAddress);

  for (std::size_t i = 0; i < Parsing::DOLPaddingSize; ++i) {
    REQUIRE(header->Padding[i] == Padding[i]);
    REQUIRE(header->Padding[i] == Padding[i]);
    REQUIRE(header->Padding[i] == Padding[i]);
  }
}
