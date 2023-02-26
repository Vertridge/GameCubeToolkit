#include "Disassembler/Program.h"

#include <cassert>

using namespace PowerPC;

Instruction::Instruction(std::string name, std::uint32_t address)
    : mName(name), mAddress(address) {}

void Instruction::Dump(std::ostream &os) const {
  os << "0x" << std::hex << mAddress << "\t" << mName << "\n";
}

Function::Function(std::string name, std::uint32_t address)
    : mName(name), mAddress(address) {}

void Function::Dump(std::ostream &os) const {
  os << mName << "\n";
  for (const auto &instr : mInstructions) {
    instr->Dump(os);
  }
}

void TextSection::AddFunction(std::uint32_t addr,
                              std::shared_ptr<Function> func) {

  mFunctions.insert(std::make_pair(addr, std::move(func)));
}

bool TextSection::AddressInSection(std::uint32_t address) {
  return address > mAddress && address < (mAddress + mSize);
}

TextSection::TextSection(std::uint32_t offset, std::uint32_t address,
                         std::uint32_t size)
    : mOffset(offset), mAddress(address), mSize(size) {}

void TextSection::Dump(std::ostream &os) const {
  os << mName << "\n";
  for (const auto &[addr, func] : mFunctions) {
    func->Dump(os);
  }
}

void TextSection::DumpInstructions(std::ostream &os) const {
  for (const auto &instr : mInstructions) {
    instr->Dump(os);
  }
}

void DataSection::Dump(std::ostream &os) const { os << mName << "\n"; }

void Program::AddTextSection(TextSection section) {
  mTextSections.push_back(std::move(section));
}

void Program::AddDataSection(DataSection section) {
  mDataSections.push_back(std::move(section));
}

TextSection &Program::GetTextSection(std::size_t i) {
  assert(i < mTextSections.size());
  return mTextSections[i];
}

DataSection &Program::GetDataSection(std::size_t i) { return mDataSections[i]; }

void Program::Dump(std::ostream &os) const {
  os << mName << "\n";
  for (const auto &textSection : mTextSections) {
    textSection.Dump(os);
  }
}
