#pragma once

#include <int24.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Parsing {

struct __attribute__((packed)) FSTEntry {
  std::uint8_t flags;          // File: 0, Dir: 1
  util::uint24 fileNameOffset; // offset into string table
  // Offset of the file, parent offset for dir
  union {
    std::uint32_t fileOffset;
    std::uint32_t parentOffset;
  };
  // lenght of the file, number of entries (root) or
  // next offset (dir)
  union {
    std::uint32_t fileLen;
    std::uint32_t numEntries;
    std::uint32_t nextOffset; // last file of the current dir.
  };
};

class FST {
public:
  bool Parse(std::uint8_t *table, std::size_t length);
  void Print(std::ostream &os);
  void PrintStrings(std::ostream &os);

  const std::string &GetString(std::size_t index);
  const FSTEntry &GetEntry(std::size_t index);

  std::vector<FSTEntry>::iterator begin();
  std::vector<FSTEntry>::iterator end();

private:
  FSTEntry ParseEntry(std::uint8_t *entry);
  void PrintEntry(const FSTEntry &entry, std::size_t index, std::ostream &os);
  void ParseStrings(std::uint8_t *stringTable, std::uint8_t *end);
  FSTEntry mRootDirEntry;
  std::vector<FSTEntry> mEntries;
  std::map<std::size_t, std::string> mStringTable;
};

} // namespace Parsing
