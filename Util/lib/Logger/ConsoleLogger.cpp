#include "Logger/ConsoleLogger.h"

#include <iostream>

namespace util {

void ConsoleLogger::Log(Verbosity verbosity, const std::string &str) {
  switch (verbosity) {
  case Verbosity::ERROR:
    std::cerr << "\033[1;31m"
              << "[ERROR] " << str << "\033[0m\n";
    break;
  case Verbosity::INFO:
    std::cout << "[INFO] " << str << "\n";
    break;
  case Verbosity::WARN:
    std::cout << "\033[1;33m"
              << "[WARNING] " << str << "\033[0m\n";
    break;
  case Verbosity::TRACE:
    std::cout << "\033[1;90m"
              << "[TRACE] " << str << "\033[0m\n";
    break;
  }
}
} // namespace util
