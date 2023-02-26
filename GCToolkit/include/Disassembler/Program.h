#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace PowerPC {

class Function;

class Instruction {
public:
  Instruction(std::string name, std::uint32_t address);
  Instruction(Instruction &) = delete;
  Instruction(Instruction &&) = delete;

  Instruction &operator=(Instruction &) = delete;
  Instruction &operator=(Instruction &&) = delete;

  std::string_view GetName() { return mName; }
  std::uint32_t GetAddress() { return mAddress; }
  std::weak_ptr<Function> GetParent() { return mFunction; }
  void SetParent(std::weak_ptr<Function> parent) {
    assert(mFunction.expired() && "Parent already set.");
    mFunction = parent;
  }
  bool HasParent() { return !mFunction.expired(); }

  void Dump(std::ostream &os) const;

private:
  std::string mName;
  std::uint32_t mAddress;
  std::weak_ptr<Function> mFunction;
};

class Function {
public:
  Function(std::string name, std::uint32_t address);
  Function(Function &) = delete;
  Function(Function &&) = delete;

  Function &operator=(Function &) = delete;
  Function &operator=(Function &&) = delete;

  std::uint32_t GetAddress() { return mAddress; }
  std::string_view GetName() { return mName; }

  void Add(std::shared_ptr<Instruction> &instr) {
    mInstructions.push_back(instr);
  }
  std::vector<std::shared_ptr<Instruction>>::iterator begin() {
    return mInstructions.begin();
  }
  std::vector<std::shared_ptr<Instruction>>::iterator end() {
    return mInstructions.end();
  }

  void Dump(std::ostream &os) const;

private:
  std::string mName;
  std::uint32_t mAddress;
  std::vector<std::shared_ptr<Instruction>> mInstructions;
};

class TextSection {
public:
  TextSection(std::uint32_t offset, std::uint32_t address, std::uint32_t size);

  std::string_view GetName() { return mName; }
  void SetName(std::string name) { mName = name; }
  std::uint32_t GetOffset() { return mOffset; }
  std::uint32_t GetAddress() { return mAddress; }
  std::uint32_t GetSize() { return mSize; }

  void AddFunction(std::uint32_t address, std::shared_ptr<Function> func);

  std::shared_ptr<Function> &GetFunction(std::uint32_t addr) {
    return mFunctions.at(addr);
  }
  bool HasFunctionAt(std::uint32_t address) {
    return mFunctions.find(address) != mFunctions.end();
  }
  std::map<const std::uint32_t, std::shared_ptr<Function>>::iterator begin() {
    return mFunctions.begin();
  }
  std::map<const std::uint32_t, std::shared_ptr<Function>>::iterator end() {
    return mFunctions.end();
  }

  std::vector<std::shared_ptr<Instruction>> &GetInstructions() {
    return mInstructions;
  }

  const std::vector<std::shared_ptr<Instruction>> &GetInstructions() const {
    return mInstructions;
  }

  bool AddressInSection(std::uint32_t address);

  void Dump(std::ostream &os) const;
  void DumpInstructions(std::ostream &os) const;

private:
  std::string mName = "";
  std::uint32_t mOffset = 0;
  std::uint32_t mAddress = 0;
  std::uint32_t mSize = 0;

  std::map<std::uint32_t, std::shared_ptr<Function>> mFunctions;
  std::vector<std::shared_ptr<Instruction>> mInstructions;
};

class DataSection {
public:
  std::string_view GetName() { return mName; }
  std::uint32_t GetOffset() { return mOffset; }
  std::uint32_t GetAddress() { return mAddress; }
  std::uint32_t GetSize() { return mSize; }

  void Dump(std::ostream &os) const;

private:
  std::string mName;
  std::uint32_t mOffset = 0;
  std::uint32_t mAddress = 0;
  std::uint32_t mSize = 0;
};

class Program {
public:
  void AddTextSection(TextSection section);
  void AddDataSection(DataSection section);
  TextSection &GetTextSection(std::size_t i);
  DataSection &GetDataSection(std::size_t i);

  void SetName(std::string name) { mName = name; }
  std::string_view GetName() { return mName; }

  void Dump(std::ostream &os) const;

private:
  std::vector<TextSection> mTextSections;
  std::vector<DataSection> mDataSections;
  std::string mName;
};

void ParseInstructionToFunctions(TextSection &section);

} // namespace PowerPC
