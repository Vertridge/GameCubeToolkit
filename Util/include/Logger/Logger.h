#pragma once

#include "LoggerBase.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

// vendor
#include <fmt/core.h>

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
  void RemoveLogger(LoggerBase *logger);

  ~Logger();

private:
  Logger() = default;
  std::vector<LoggerBase *> mLoggers;
};

#define LOG_ERROR(...)                                                         \
  util::Logger::GetSingleton().Log(util::Verbosity::ERROR,                     \
                                   fmt::format(__VA_ARGS__))

#define LOG_WARN(...)                                                          \
  util::Logger::GetSingleton().Log(util::Verbosity::WARN,                      \
                                   fmt::format(__VA_ARGS__))

#define LOG_INFO(...)                                                          \
  util::Logger::GetSingleton().Log(util::Verbosity::INFO,                      \
                                   fmt::format(__VA_ARGS__))

#define LOG_TRACE(...)                                                         \
  util::Logger::GetSingleton().Log(util::Verbosity::TRACE,                     \
                                   fmt::format(__VA_ARGS__))

} // namespace util
