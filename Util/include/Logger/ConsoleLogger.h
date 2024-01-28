#pragma once

#include "LoggerBase.h"

#include <string>

namespace util {

class ConsoleLogger : public LoggerBase {
public:
  virtual void Log(Verbosity verbosity, const std::string &str);
  ~ConsoleLogger() = default;
};

} // namespace util
