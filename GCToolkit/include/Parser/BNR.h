#pragma once

// stl
#include <cstdint>
#include <istream>
#include <string>
#include <vector>

namespace Parsing {

constexpr auto bnr_magic_ntsc = "BNR1";
constexpr auto bnr_magic_pal = "BNR2";
constexpr auto bnr_magic_size = 4;

constexpr auto bnr_padding_size = 0x1c;
constexpr auto bnr_graphical_data_size = 0x1800;
constexpr auto bnr_game_name_size = 0x20;
constexpr auto bnr_company_name_size = 0x20;
constexpr auto bnr_full_game_name_size = 0x40;
constexpr auto bnr_full_company_name_size = 0x20;
constexpr auto bnr_description_size = 0x80;

struct BNRHeader {
  std::uint8_t magic[bnr_magic_size];
  std::uint8_t padding[bnr_padding_size];
  std::uint8_t graphicData[bnr_graphical_data_size];
  std::uint8_t gameName[bnr_game_name_size];
  std::uint8_t companyName[bnr_company_name_size];
  std::uint8_t fullGameName[bnr_full_game_name_size];
  std::uint8_t fullCompanyName[bnr_full_company_name_size];
  // TODO in PAL games this the description repeats multiple times, once per
  // language.
  std::uint8_t gameDescription[bnr_description_size];
};

class BNRFile {
public:
  BNRFile() = default;
  BNRFile(const std::string &file);
  bool Parse(const std::string &file);
  bool Parse(std::istream &stream);
  bool ParseHeader(std::istream &input);
  void PrintHeader(std::ostream &os);

  BNRHeader *GetHeader() { return mHeader; }
  const BNRHeader *GetHeader() const { return mHeader; }

  const std::string &GetFileName() const { return mFileName; }

private:
  BNRHeader *mHeader;
  std::vector<std::uint8_t> mHeaderBuffer;
  std::string mFileName;
};

} // namespace Parsing
