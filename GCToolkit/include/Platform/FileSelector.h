#pragma once

// std
#include <string>

namespace Platform::FileSelector {

std::string SelectFile(const std::string &title = "");
std::string SelectDirectory(const std::string &title = "");
} // namespace Platform::FileSelector
