#include "String.h"

namespace util {

bool CaseInsensitiveCharCompare(char a, char b) {
  return std::tolower(static_cast<unsigned char>(a)) ==
         std::tolower(static_cast<unsigned char>(b));
}

bool CaseInsensitiveCompare(const std::string &str1, const std::string &str2) {
  return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(),
                    CaseInsensitiveCharCompare);
}

} // namespace util
