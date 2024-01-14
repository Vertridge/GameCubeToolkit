#pragma once

#include <string>

namespace Util {

class Error {
public:
  Error();
  Error(const std::string &error);
  ~Error();

  static Error Success();

  bool HasError() const;

  operator bool() const;

private:
  Error(bool err);
  bool mError = false;
  mutable bool mChecked = false;
  std::string mMsg = "";
};

Error MakeError(std::string err);

} // namespace Util
