#pragma once

#include <filesystem>

namespace ge {

class ReadOnlyFileMap {
public:
    ReadOnlyFileMap() {}
    ReadOnlyFileMap(const std::filesystem::path& filePath);
    ReadOnlyFileMap(const ReadOnlyFileMap& other) = delete;
    ReadOnlyFileMap(ReadOnlyFileMap&& other) = default;
    ~ReadOnlyFileMap();

    void map(const std::filesystem::path& filePath);
    void unmap();

private:
    void* _addr = nullptr;
    size_t _length = 0;
};

} // namespace ge
