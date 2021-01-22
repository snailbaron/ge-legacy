#pragma once

#include <filesystem>
#include <span>
#include <vector>

namespace ge {

std::vector<unsigned char> slurp(const std::filesystem::path& path);
void write(
    const std::vector<unsigned char>& data, const std::filesystem::path& path);
void write(
    const std::span<const std::byte>& bytes, const std::filesystem::path& path);

} // namespace ge
