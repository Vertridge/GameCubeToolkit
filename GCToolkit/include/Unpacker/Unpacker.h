#pragma once

#include "Parser/FST.h"
#include "Parser/GCM.h"

namespace Unpacking {

struct UnpackerOptions {
  std::string inputISO;
  std::string dump;
  std::string outDir;
};

class Unpacker {
public:
  Unpacker(UnpackerOptions options);

  bool Unpack();
  void Dump();

  const Parsing::GCMFile &GetGcm() const { return mGcm; }
  Parsing::GCMFile &GetGcm() { return mGcm; }

private:
  Parsing::GCMFile mGcm;
  UnpackerOptions mOptions;
};

} // namespace Unpacking
