#pragma once

#include "Parser/FST.h"
#include "Parser/GCM.h"

namespace Unpacking {

struct RepackerOptions {
  std::string outputIso;
  std::string dump;
  std::string inDir;
};

class Repacker {
public:
  Repacker(RepackerOptions options);

  bool Repack();
  void Dump();

  const Parsing::GCMFile &GetGcm() const { return mGcm; }
  Parsing::GCMFile &GetGcm() { return mGcm; }

private:
  Parsing::GCMFile mGcm;
  RepackerOptions mOptions;
};

} // namespace Unpacking
