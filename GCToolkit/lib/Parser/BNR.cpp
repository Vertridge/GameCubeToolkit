#include "Parser/BNR.h"

// util
#include <Logger/Logger.h>

// stl
#include <cstring>
#include <fstream>
#include <iomanip>

namespace Parsing {

BNRFile::BNRFile(const std::string &file) { Parse(file); }

bool BNRFile::Parse(std::istream &stream) {
  if (stream.peek() == EOF) {
    LOG_ERROR("Failed to open: '{}' - {}", mFileName, std::strerror(errno));
    return false;
  }
  LOG_TRACE("Parsing BNR header");

  ParseHeader(stream);
  return true;
}

bool BNRFile::Parse(const std::string &file) {
  mFileName = file;
  LOG_INFO("Parsing BNR File: {}", mFileName);
  std::ifstream input(file.c_str(), std::ios::binary);
  if (!input.is_open()) {
    LOG_ERROR("Failed to open: '{}' - {}", mFileName, std::strerror(errno));
    return false;
  }
  return Parse(input);
}

bool BNRFile::ParseHeader(std::istream &input) {
  mHeaderBuffer.resize(sizeof(BNRHeader));
  input.read(reinterpret_cast<char *>(mHeaderBuffer.data()),
             mHeaderBuffer.size());
  mHeader = reinterpret_cast<BNRHeader *>(mHeaderBuffer.data());
  if (!std::memcmp(mHeader->magic, bnr_magic_pal, bnr_magic_size) &&
      !std::memcmp(mHeader->magic, bnr_magic_ntsc, bnr_magic_size)) {
    LOG_ERROR("BNR Magic did not match BNR1 or BNR2: {}",
              reinterpret_cast<char *>(mHeader->magic));
    return false;
  }
  return true;
}

void BNRFile::PrintHeader(std::ostream &os) {
  // clang-format off
  os << "Magic: ";
  for (auto i = 0; i < bnr_magic_size; ++i) {
    os << mHeader->magic[i];
  }
  os << "\n";

  os << "Game Name: \n\t";
  for (auto i = 0; i < bnr_game_name_size; ++i) {
    if(mHeader->gameName[i] == '\0') {
      break;
    }
    os << mHeader->gameName[i];
  }
  os << "\n";

  os << "Company Name: \n\t";
  for (auto i = 0; i < bnr_company_name_size; ++i) {
    if(mHeader->companyName[i] == '\0') {
      break;
    }
    os << mHeader->companyName[i];
  }
  os << "\n";

  os << "Full Game Name: \n\t";
  for (auto i = 0; i < bnr_full_game_name_size; ++i) {
    if(mHeader->fullGameName[i] == '\0') {
      break;
    }
    os << mHeader->fullGameName[i];
  }
  os << "\n";

  os << "Full Company Name: \n\t";
  for (auto i = 0; i < bnr_full_company_name_size; ++i) {
    if(mHeader->fullCompanyName[i] == '\0') {
      break;
    }
    os << mHeader->fullCompanyName[i];
  }
  os << "\n";

  os << "Description: \n\t";
  for (auto i = 0; i < bnr_description_size; ++i) {
    if(mHeader->gameDescription[i] == '\0') {
      continue;
    }
    os << mHeader->gameDescription[i];
  }
  os << "\n";

  os << "Padding: \n\t";
  int newlineCounter = 0;
  for(auto i = 0; i < bnr_padding_size; ++i) {
    os << "0x"  << std::setw(2) << std::setfill('0') << std::hex
       << (int)(mHeader->padding[i] & 0xff) << " ";
    ++newlineCounter;
    if(newlineCounter == 20) {
      os << "\n\t";
      newlineCounter = 0;
    }
  }
  os << "\n";

  os << "Graphics: \n\t";
  newlineCounter = 0;
  for(auto i = 0; i < bnr_graphical_data_size; ++i) {
    os << "0x"  << std::setw(2) << std::setfill('0') << std::hex
       << (int)(mHeader->graphicData[i] & 0xff) << " ";
    ++newlineCounter;
    if(newlineCounter == 10) {
      os << "\n\t";
      newlineCounter = 0;
    }
  }
  os << "\n";

  os << std::endl;
  // clang-format on
}

} // namespace Parsing
