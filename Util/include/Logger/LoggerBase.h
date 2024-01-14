#pragma once

#include <string>

namespace util {

enum class Verbosity { TRACE, INFO, WARN, ERROR };

class LoggerBase {
public:
  virtual void Log(Verbosity verbosity, const std::string &str) const = 0;
  virtual ~LoggerBase() = default;
};

} // namespace util
