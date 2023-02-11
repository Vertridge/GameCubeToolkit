#include <iostream>

#include <Parser/DOL.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Pass 2 file to patch" << std::endl;
    return 1;
  }

  std::cout << "DOLPatcher: " << argv[1] << std::endl;

  Patcher::Parsing::DOLFile dol;
  dol.Parse(argv[1]);
  dol.PrintHeader();

  return 0;
}
