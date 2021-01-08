#pragma once

#include <filesystem>
#include <vector>

std::vector<unsigned char> slurp(const std::filesystem::path& path);
