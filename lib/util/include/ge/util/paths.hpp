#pragma once

#include <filesystem>

namespace ge {

std::filesystem::path currentExecutablePath();
std::filesystem::path currentExecutableDirectory();

bool isFile(const std::filesystem::path& path);

} // namespace ge
