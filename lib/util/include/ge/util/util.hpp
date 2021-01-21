#pragma once

#include <filesystem>
#include <vector>

namespace ge {

std::vector<unsigned char> slurp(const std::filesystem::path& path);

} // namespace ge
