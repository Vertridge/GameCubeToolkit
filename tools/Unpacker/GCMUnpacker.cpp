#include <fstream>
#include <iostream>

#include "Parser/GCM.h"

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Pass File to Unpack" << std::endl;
    return 1;
  }

  std::cout << "GCM Unpacker: " << argv[1] << std::endl;

  Parsing::GCMFile gcm;

  if (!gcm.Parse(argv[1])) {
    std::cerr << "Failed to open: " << argv[1] << std::endl;
    return 1;
  }

  std::string output = "gcmDump.txt";
  if (argc >= 3) {
    output = argv[2];
  }

  std::ofstream ofstrm(output, std::ios::binary);

  gcm.PrintBootHeader(ofstrm);
  gcm.PrintDiskHeader(ofstrm);
  gcm.PrintAppLoaderHeader(ofstrm);

  ofstrm.close();

  std::ofstream bootofstrm(Parsing::BootFileName, std::ios::binary);
  gcm.WriteBootHeader(bootofstrm);
  bootofstrm.close();

  std::ofstream diskofstrm(Parsing::DiskFileName, std::ios::binary);
  gcm.WriteDiskHeader(diskofstrm);
  diskofstrm.close();

  std::ofstream applofstrm(Parsing::AppLoaderFileName, std::ios::binary);
  gcm.WriteAppLoaderHeader(applofstrm);
  applofstrm.close();
}
