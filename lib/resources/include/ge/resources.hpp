#pragma once

#include "schema_generated.h"

#include <array>
#include <cstddef>
#include <filesystem>
#include <span>

namespace ge {

/*
class Resources {
public:
    Resources(const std::filesystem::path& resourceFilePath);

    ge::Sprite load(SpriteId spriteId) const;
    ge::Font load(FontId fontId) const;

private:
    ReadOnlyFileMap _data;
    ge::schema::Resources* _root = nullptr;
};




class DataPacker {
public:
    void addFont(const std::filesystem::path& fontFilePath);
    void addPng(const std::filesystem::path& pngFilePath, int frameCount);
    void writeToFile(const std::filesystem::path& outputFilePath);

private:
};

class DataReader {
public:
    DataReader(const std::filesystem::path& resourceFilePath);

    std::span<std::byte> sheet() const;

    size_t fontCount() const;
    std::span<std::byte> font(size_t i) const;
private:
};
*/

} // namespace ge
