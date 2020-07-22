#pragma once

#include <filesystem>
#include <vector>

namespace ge {

enum class ResourceType {
    ASEPRITE,
    PNG,
    TTF,
};

class ResourcePacker {
public:
    ResourcePacker& add(
        const std::filesystem::path& resourceFilePath,
        ResourceType resourceType);
    ResourcePacker& add(const std::filesystem::path& resourceFilePath);

    void pack(
        const std::filesystem::path& targetPath,
        const std::filesystem::path& asepritePath);

private:
    std::vector<std::filesystem::path> _pngPaths;
    std::vector<std::filesystem::path> _ttfPaths;
};

} // namespace ge
