#include "Error.h"

#include <cassert>

namespace Util {
Error::Error() : mError(true), mChecked(false) {}
Error::Error(const std::string &error)
    : mError(true), mChecked(false), mMsg(error) {}

// If there is no error there is no need to check.
Error::Error(bool err) : mError(err), mChecked(!err) {}

Error::~Error() {
  assert(mChecked && "Error was not checked before being destroyed");
}

bool Error::HasError() const {
  mChecked = true;
  return mError;
}
Error::operator bool() const {
  mChecked = true;
  return mError;
}

Error Error::Success() { return Error(false); }

Error MakeError(std::string err) { return Error(err); }

} // namespace Util
