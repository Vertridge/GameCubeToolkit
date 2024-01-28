#include "Logger/Logger.h"

#include <vector>

namespace util {

void Logger::AddLogger(LoggerBase *logger) { mLoggers.emplace_back(logger); }

void Logger::RemoveLogger(LoggerBase *logger) {
  auto loggerToRemove = std::find(mLoggers.begin(), mLoggers.end(), logger);
  if (loggerToRemove == mLoggers.end()) {
    return;
  }
  mLoggers.erase(loggerToRemove);
}

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
