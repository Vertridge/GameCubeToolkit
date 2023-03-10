#include "Disassembler/Program.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

using namespace PowerPC;

Instruction::Instruction(std::string name, std::uint32_t address,
                         ppc_insn opcode)
    : mName(name), mAddress(address), mOpcode(opcode) {}

void Instruction::Dump(std::ostream &os) const {
  os << "0x" << std::hex << mAddress << "\t" << mName << "\n";
}

BranchInstruction::BranchInstruction(std::string name, std::uint32_t address,
                                     ppc_insn opcode)
    : Instruction(name, address, opcode) {}

BranchInstruction::BranchInstruction(std::string name, std::uint32_t address,
                                     ppc_insn opcode, std::uint32_t callAddress)
    : Instruction(name, address, opcode), mCallAddress(callAddress) {}

void BranchInstruction::Dump(std::ostream &os) const {
  auto callFunction = mCallFunction.lock();
  if (mCallFunction.expired()) {
    Instruction::Dump(os);
    return;
  }
  std::stringstream stream;
  Instruction::Dump(stream);
  auto string = stream.str();
  if (string.back() == '\n') {
    string.pop_back();
  }
  os << string << "\t" << mCallFunction.lock()->GetName() << "\n";
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

bool TextSection::AddressInSection(std::uint32_t address) const {
  return address >= mAddress && address < (mAddress + mSize);
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
  section.SetParent(this);
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

TextSection *Program::GetTextSectionWithAddress(std::uint32_t address) {
  auto section = std::find_if(
      mTextSections.begin(), mTextSections.end(),
      [&](const auto &section) { return section.AddressInSection(address); });
  if (section != mTextSections.end()) {
    return &(*section);
  }
  std::cout << "Could not find section for: 0x" << std::hex << address << "\n";
  return nullptr;
}

std::shared_ptr<Function>
CreateFunction(std::uint32_t address,
               std::optional<std::string> name = std::nullopt) {
  std::stringstream stream;

  if (name.has_value()) {
    stream << *name << "_0x";
  } else {
    stream << "function_0x";
  }

  stream << std::hex << address;

  return std::make_shared<Function>(stream.str(), address);
}

void PowerPC::ParseInstructionToFunctions(Program &program) {

  auto entryFunc = CreateFunction(program.GetEntryPoint(), "start");
  auto entrySection =
      program.GetTextSectionWithAddress(program.GetEntryPoint());
  entrySection->AddFunction(program.GetEntryPoint(), entryFunc);

  for (auto &textSection : program.GetTextSections()) {
    auto &instructions = textSection.GetInstructions();

    for (auto &instr : instructions) {
      if (instr->GetOpcode() != PPC_INS_BL) {
        continue;
      }

      auto branchInstr = std::static_pointer_cast<BranchInstruction>(instr);

      auto branchAddr = branchInstr->GetCallAddress();
      auto *funcSect = program.GetTextSectionWithAddress(branchAddr);

      if (funcSect == nullptr) {
        continue;
      }

      if (funcSect->HasFunctionAt(branchAddr)) {
        auto func = funcSect->GetFunction(branchAddr);
        branchInstr->SetCallFunction(func);
        continue;
      }

      auto func = CreateFunction(branchAddr);

      funcSect->AddFunction(branchAddr, func);
      branchInstr->SetCallFunction(func);
      func->AddCallee(branchInstr);
    }
  }

  for (auto &textSection : program.GetTextSections()) {
    auto &instructions = textSection.GetInstructions();

    if (instructions.size() == 0) {
      continue;
    }

    auto tempfunction = textSection.begin()->second;

    for (auto &instr : instructions) {
      if (textSection.HasFunctionAt(instr->GetAddress())) {
        tempfunction.reset();
        tempfunction = textSection.GetFunction(instr->GetAddress());
      }
      instr->SetParent(tempfunction);
      tempfunction->Add(instr);
    }
  }
}
