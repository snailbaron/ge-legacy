#pragma once

#include "schema_generated.h"

#include <filesystem>
#include <vector>

namespace ge {

class SpriteId {
public:
    explicit SpriteId(size_t id) : _id(id) {}
    operator size_t() const { return _id; }

private:
    size_t _id = 0;
};

class FontId {
public:
    explicit FontId(size_t id) : _id(id) {}
    operator size_t() const { return _id; }

private:
    size_t _id = 0;
};

class ResourceWriter {
public:
    SpriteId addSprite(
        const std::filesystem::path& path,
        int frameCount = 1,
        int frameMs = 250);
    FontId addFont(const std::filesystem::path& path);
    void write(std::ostream& output);

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
