#include "Parser/DOL.h"

#include "Binary.h"

// Util
#include <Logger/Logger.h>

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace Parsing;

DOLFile::DOLFile(const std::string &file) { Parse(file); }

bool DOLFile::Parse(std::istream &stream) {
  if (stream.peek() == EOF) {
    LOG_ERROR("Failed to open: '{}' - {}", mFileName, std::strerror(errno));
    return false;
  }
  LOG_TRACE("Parsing header");

  ParseHeader(stream);
  ReadData(stream);
  return true;
}

bool DOLFile::Parse(const std::string &file) {
  mFileName = file;
  LOG_INFO("Parsing File: {}", mFileName);
  std::ifstream input(file.c_str(), std::ios::binary);
  if (!input.is_open()) {
    LOG_ERROR("Failed to open: '{}' - {}", mFileName, std::strerror(errno));
    return false;
  }
  return Parse(input);
}

bool DOLFile::ParseHeader(std::istream &input) {
  mHeaderBuffer.resize(sizeof(DOLHeader));
  input.read(reinterpret_cast<char *>(mHeaderBuffer.data()),
             mHeaderBuffer.size());
  mHeader = reinterpret_cast<DOLHeader *>(mHeaderBuffer.data());

  // Swap endiness.
  for (std::size_t i = 0; i < DOLNumberOfTextSections; ++i) {
    util::SwapBinary(mHeader->TextSectionOffsets[i]);
  }
  for (std::size_t i = 0; i < DOLNumberOfDataSections; ++i) {
    util::SwapBinary(mHeader->DataSectionOffsets[i]);
  }
  for (std::size_t i = 0; i < DOLNumberOfTextSections; ++i) {
    util::SwapBinary(mHeader->TextSectionAddresses[i]);
  }
  for (std::size_t i = 0; i < DOLNumberOfDataSections; ++i) {
    util::SwapBinary(mHeader->DataSectionAddresses[i]);
  }
  for (std::size_t i = 0; i < DOLNumberOfTextSections; ++i) {
    util::SwapBinary(mHeader->TextSectionSizes[i]);
  }
  for (std::size_t i = 0; i < DOLNumberOfDataSections; ++i) {
    util::SwapBinary(mHeader->DataSectionSizes[i]);
  }
  util::SwapBinary(mHeader->BSSAddress);
  util::SwapBinary(mHeader->BSSSize);
  util::SwapBinary(mHeader->EntryPointAddress);
  for (std::size_t i = 0; i < DOLPaddingSize; ++i) {
    util::SwapBinary(mHeader->Padding[i]);
  }
  return true;
}

bool DOLFile::ReadData(std::istream &input) {
  // Save begin position to start reading.
  auto beginpos = input.tellg();
  input.seekg(0, std::ios::end);
  // Get size of data.
  auto size = input.tellg() - beginpos;

  // Rest file position to starting point.
  input.seekg(beginpos);
  mDataBuffer.resize(size);
  input.read(reinterpret_cast<char *>(mDataBuffer.data()), mDataBuffer.size());
  return true;
}

void DOLFile::PrintHeader(std::ostream &os) {
  // clang-format off
  os << "TextSection Offsets: " << "\n";
  for (std::size_t i = 0; i < DOLNumberOfTextSections; ++i) {
    os << " 0x" << std::hex << mHeader->TextSectionOffsets[i] 
              << " " << std::dec << mHeader->TextSectionOffsets[i] << "\n";
  }
  os << "DataSection Offsets: " << "\n";
  for (std::size_t i = 0; i < DOLNumberOfDataSections; ++i) {
    os << " 0x" << std::hex << mHeader->DataSectionOffsets[i] 
              << " " << std::dec << mHeader->DataSectionOffsets[i] << "\n";
  }
  os << "TextSection Addresses: " << "\n";
  for (std::size_t i = 0; i < DOLNumberOfTextSections; ++i) {
    os << " 0x" << std::hex << mHeader->TextSectionAddresses[i] 
              << " " << std::dec << mHeader->TextSectionAddresses[i] << "\n";
  }
  os << "DataSection Addresses: " << "\n";
  for (std::size_t i = 0; i < DOLNumberOfDataSections; ++i) {
    os << " 0x" << std::hex << mHeader->DataSectionAddresses[i] 
              << " " << " " << std::dec << mHeader->DataSectionAddresses[i] << "\n";
  }
  os << "TextSection Sizes: " << "\n";
  for (std::size_t i = 0; i < DOLNumberOfTextSections; ++i) {
    os << " 0x" << std::hex << mHeader->TextSectionSizes[i] 
              << " " << std::dec << mHeader->TextSectionSizes[i] << "\n";
  }
  os << "DataSection Sizes: " << "\n";
  for (std::size_t i = 0; i < DOLNumberOfDataSections; ++i) {
    os << " 0x" << std::hex << mHeader->DataSectionSizes[i] 
              << " " << std::dec << mHeader->DataSectionSizes[i] << "\n";
  }
  os << "BSS Address: 0x" << std::hex << mHeader->BSSAddress 
                        << " " << std::dec << mHeader->BSSAddress << "\n";
  os << "BSS Size: 0x" << std::hex << mHeader->BSSSize
                     << " " << std::dec << mHeader->BSSSize << "\n";
  os << "Entry Point: 0x" << std::hex << mHeader->EntryPointAddress
                        << " " << std::dec << mHeader->EntryPointAddress << "\n";
  os << "Padding: \n";
  for (std::size_t i = 0; i < DOLPaddingSize; ++i) {
    os << " 0x" << std::hex << mHeader->Padding[i]
              << " " << std::dec << mHeader->Padding[i] << "\n";
  }
  os << std::endl;
  // clang-format on
}

void DOLFile::PrintData(std::ostream &os, std::size_t begin, std::size_t size) {
  if (size == 0) {
    size = mDataBuffer.size() - begin;
  }

  for (auto i = begin; i < (begin + size); ++i) {
    os << std::setw(2) << std::setfill('0') << std::hex
       << (int)(mDataBuffer[i] & 0xff) << " ";
  }
  os << std::endl;
}
