#include "Logger/Logger.h"

namespace util {

void Logger::AddLogger(LoggerBase *logger) { mLoggers.emplace_back(logger); }

void Logger::Log(Verbosity verbosity, const std::string &str) {
  for (const auto &logger : mLoggers) {
    logger->Log(verbosity, str);
  }
}

Logger::~Logger() {
  for (auto &logger : mLoggers) {
    delete logger;
  }
}
} // namespace util
