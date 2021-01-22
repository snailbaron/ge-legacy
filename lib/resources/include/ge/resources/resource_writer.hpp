#pragma once

#include "ge/resources/ids.hpp"

#include "schema_generated.h"

#include <filesystem>
#include <vector>

namespace ge {

class ResourceWriter {
public:
    SpriteId addSprite(
        const std::filesystem::path& path,
        int frameCount = 1,
        int frameMs = 250);
    FontId addFont(const std::filesystem::path& path);
    void write(std::ostream& output);
    void write(const std::filesystem::path& outputFilePath);

private:
    struct SpriteFileInfo {
        std::filesystem::path path;
        int frameCount = 0;
        int frameMs = 0;
    };

    flatbuffers::FlatBufferBuilder _builder;
    std::vector<flatbuffers::Offset<schema::Sprite>> _sprites;
    std::vector<flatbuffers::Offset<schema::Font>> _fonts;

    std::vector<SpriteFileInfo> _spriteFilesInfo;
    int maxSpriteWidth = 0;
    int totalSpriteHeight = 0;
};

} // namespace ge
