#include "Parser/FST.h"

#include "Util/Binary.h"

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace Parsing;

bool FST::Parse(std::uint8_t *table, std::size_t length) {
  std::cerr << "Parsing FST" << std::endl;
  if (table == nullptr) {
    std::cerr << "FST table pointer is a nullptr";
    return false;
  }
  std::cerr << "Size of entry: " << sizeof(FSTEntry) << std::endl;

  // First parse root dir entry
  mRootDirEntry = ParseEntry(table);
  std::uint8_t *tablePtr = table + sizeof(FSTEntry);
  // For the root entry filelen is the number of entries.
  for (std::uint32_t i = 1; i < mRootDirEntry.fileLen; ++i) {
    mEntries.push_back(ParseEntry(tablePtr));
    tablePtr += sizeof(FSTEntry);
  }
  ParseStrings(tablePtr, table + length);

  return true;
}

FSTEntry FST::ParseEntry(std::uint8_t *entryPtr) {
  FSTEntry entry;
  std::memcpy(&entry, entryPtr, sizeof(FSTEntry));
  util::SwapBinary(entry.fileNameOffset);
  util::SwapBinary(entry.fileOffset);
  util::SwapBinary(entry.fileLen);
  return entry;
}

void FST::ParseStrings(std::uint8_t *stringTable, std::uint8_t *end) {
  auto *ptr = stringTable;
  std::size_t offset = 0;
  while (ptr < end) {
    std::string str(reinterpret_cast<char *>(ptr));
    auto diff = str.length() + 1;
    ptr += diff;
    mStringTable.insert({offset, std::move(str)});
    offset += diff;
  }
}

const std::string &FST::GetString(std::size_t index) {
  return mStringTable[index];
}

const FSTEntry &FST::GetEntry(std::size_t index) { return mEntries[index]; }

std::vector<FSTEntry>::iterator FST::begin() { return mEntries.begin(); }

std::vector<FSTEntry>::iterator FST::end() { return mEntries.end(); }

void FST::Print(std::ostream &os) {
  os << "FST: \n";
  os << "Root entry: 1\n";
  os << "\tFlags: 0x" << std::setw(1) << std::setfill('0') << std::hex
     << (int)(mRootDirEntry.flags & 0xff) << "\n";
  os << "\tFile Name: 0x" << std::hex << mRootDirEntry.fileNameOffset << " - "
     << std::dec << mRootDirEntry.fileNameOffset << "\n";
  os << "\tFile Offset: 0x" << std::hex << mRootDirEntry.fileOffset << " - "
     << std::dec << mRootDirEntry.fileOffset << "\n";
  os << "\tNumber Of Entries: 0x" << std::hex << mRootDirEntry.fileLen << " - "
     << std::dec << mRootDirEntry.fileLen << "\n";
  std::size_t index = 2;
  for (const auto entry : mEntries) {
    PrintEntry(entry, index, os);
    ++index;
  }
}

void FST::PrintEntry(const FSTEntry &entry, std::size_t index,
                     std::ostream &os) {
  if (entry.flags & 1) {
    os << "Directory: " << index << " \n";
  } else {
    os << "File: " << index << " \n";
  }

  os << "\tFlags: 0x" << std::setw(1) << std::setfill('0') << std::hex
     << (int)(entry.flags & 0xff) << "\n";
  os << "\tFile Name: 0x" << std::hex << entry.fileNameOffset << " - "
     << std::dec << entry.fileNameOffset << "\n";

  if (entry.flags & 1) {
    // Directory
    os << "\tParent Offset: 0x" << std::hex << entry.fileOffset << " - "
       << std::dec << entry.fileOffset << "\n";
    os << "\tNextOffset: 0x" << std::hex << entry.fileLen << " - " << std::dec
       << entry.fileLen << "\n";
  } else {
    // File
    os << "\tFile Offset: 0x" << std::hex << entry.fileOffset << " - "
       << std::dec << entry.fileOffset << "\n";
    os << "\tLength: 0x" << std::hex << entry.fileLen << " - " << std::dec
       << entry.fileLen << "\n";
  }
}

void FST::PrintStrings(std::ostream &os) {
  os << "Strings: \n";
  for (const auto &[index, str] : mStringTable) {
    os << "0x" << std::hex << index << " " << str << "\n";
  }
}
