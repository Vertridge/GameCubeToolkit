#pragma once

#include "LoggerBase.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace util {

class Logger {
public:
  Logger(Logger const &) = delete;
  void operator=(Logger const &) = delete;

  static Logger &GetSingleton() {
    static Logger instance;
    return instance;
  }

  void Log(Verbosity verbosity, const std::string &str);
  void AddLogger(LoggerBase *logger);

  ~Logger();

private:
  Logger() = default;
  std::vector<LoggerBase *> mLoggers;
};

#define LOG_ERROR(msg)                                                         \
  util::Logger::GetSingleton().Log(util::Verbosity::ERROR, msg)

#define LOG_WARN(msg)                                                          \
  util::Logger::GetSingleton().Log(util::Verbosity::WARN, msg)

#define LOG_INFO(msg)                                                          \
  util::Logger::GetSingleton().Log(util::Verbosity::INFO, msg)

#define LOG_TRACE(msg)                                                         \
  util::Logger::GetSingleton().Log(util::Verbosity::TRACE, msg)

} // namespace util
