#include "Parser/DOL.h"

#include "Util/Binary.h"

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace Patcher::Parsing;

DOLFile::DOLFile(std::filesystem::path file) { Parse(file); }

bool DOLFile::Parse(std::filesystem::path file) {
  mFileName = file.string();
  std::cout << "Parsing File: " << mFileName << std::endl;
  std::ifstream input(file, std::ios::binary);
  if (!input) {
    std::cerr << "Failed to open: " << mFileName << " " << std::strerror(errno)
              << std::endl;
    return false;
  }
  ParseHeader(input);
  ReadData(input);
  input.close();
  return true;
}

bool DOLFile::ParseHeader(std::ifstream &input) {
  mHeaderBuffer.resize(sizeof(DOLHeader));
  input.read(mHeaderBuffer.data(), mHeaderBuffer.size());
  mHeader = reinterpret_cast<DOLHeader *>(mHeaderBuffer.data());

  // Swap endiness.
  for (int i = 0; i < DOLNumberOfTextSections; ++i) {
    util::SwapBinary(mHeader->TextSectionOffsets[i]);
  }
  for (int i = 0; i < DOLNumberOfDataSections; ++i) {
    util::SwapBinary(mHeader->DataSectionOffsets[i]);
  }
  for (int i = 0; i < DOLNumberOfTextSections; ++i) {
    util::SwapBinary(mHeader->TextSectionAddresses[i]);
  }
  for (int i = 0; i < DOLNumberOfDataSections; ++i) {
    util::SwapBinary(mHeader->DataSectionAddresses[i]);
  }
  for (int i = 0; i < DOLNumberOfTextSections; ++i) {
    util::SwapBinary(mHeader->TextSectionSizes[i]);
  }
  for (int i = 0; i < DOLNumberOfDataSections; ++i) {
    util::SwapBinary(mHeader->DataSectionSizes[i]);
  }
  util::SwapBinary(mHeader->BSSAddress);
  util::SwapBinary(mHeader->BSSSize);
  for (int i = 0; i < DOLPaddingSize; ++i) {
    util::SwapBinary(mHeader->Padding[i]);
  }
  return true;
}

bool DOLFile::ReadData(std::ifstream &input) {
  // Save begin position to start reading.
  auto beginpos = input.tellg();
  input.seekg(0, std::ios::end);
  // Get size of data.
  auto size = input.tellg() - beginpos;

  // Rest file position to starting point.
  input.seekg(beginpos);
  mDataBuffer.resize(size);
  input.read(mDataBuffer.data(), mDataBuffer.size());
  return true;
}

void DOLFile::PrintHeader() {
  // clang-format off
  std::cout << "Printing: " << mFileName << "\n";
  std::cout << "TextSection Offsets: " << "\n";
  for (int i = 0; i < DOLNumberOfTextSections; ++i) {
    std::cout << " 0x" << std::hex << mHeader->TextSectionOffsets[i] 
              << " " << std::dec << mHeader->TextSectionOffsets[i] << "\n";
  }
  std::cout << "DataSection Offsets: " << "\n";
  for (int i = 0; i < DOLNumberOfDataSections; ++i) {
    std::cout << " 0x" << std::hex << mHeader->DataSectionOffsets[i] 
              << " " << std::dec << mHeader->DataSectionOffsets[i] << "\n";
  }
  std::cout << "TextSection Addresses: " << "\n";
  for (int i = 0; i < DOLNumberOfTextSections; ++i) {
    std::cout << " 0x" << std::hex << mHeader->TextSectionAddresses[i] 
              << " " << std::dec << mHeader->TextSectionAddresses[i] << "\n";
  }
  std::cout << "DataSection Addresses: " << "\n";
  for (int i = 0; i < DOLNumberOfDataSections; ++i) {
    std::cout << " 0x" << std::hex << mHeader->DataSectionAddresses[i] 
              << " " << " " << std::dec << mHeader->DataSectionAddresses[i] << "\n";
  }
  std::cout << "TextSection Sizes: " << "\n";
  for (int i = 0; i < DOLNumberOfTextSections; ++i) {
    std::cout << " 0x" << std::hex << mHeader->TextSectionSizes[i] 
              << " " << std::dec << mHeader->TextSectionSizes[i] << "\n";
  }
  std::cout << "DataSection Sizes: " << "\n";
  for (int i = 0; i < DOLNumberOfDataSections; ++i) {
    std::cout << " 0x" << std::hex << mHeader->DataSectionSizes[i] 
              << " " << std::dec << mHeader->DataSectionSizes[i] << "\n";
  }
  std::cout << "BSS Address: 0x" << std::hex << mHeader->BSSAddress 
                        << " " << std::dec << mHeader->BSSAddress << "\n";
  std::cout << "BSS Size: 0x" << std::hex << mHeader->BSSSize
                     << " " << std::dec << mHeader->BSSSize << "\n";
  std::cout << "Entry Point: 0x" << std::hex << mHeader->EntryPointAddress
                        << " " << std::dec << mHeader->EntryPointAddress << "\n";
  std::cout << "Padding: \n";
  for (int i = 0; i < DOLPaddingSize; ++i) {
    std::cout << " 0x" << std::hex << mHeader->Padding[i]
              << " " << std::dec << mHeader->Padding[i] << "\n";
  }
  std::cout << std::endl;
}

void DOLFile::PrintData(std::size_t begin, std::size_t size) {
  if(size == 0) {
    size = mDataBuffer.size() - begin; 
  }
  
  for(auto i = begin; i < (begin + size); ++i) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)(mDataBuffer[i] & 0xff) << " ";
  }
  std::cout << std::endl;
}
