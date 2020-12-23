#pragma once

#include <filesystem>

namespace ge {

class ResourcePacker {
public:
    void addFont(const std::filesystem::path& fontFilePath);
};

} // namespace ge
