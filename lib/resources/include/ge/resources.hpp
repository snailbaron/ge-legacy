#pragma once

#include "schema_generated.h"

#include <filesystem>
#include <vector>

namespace ge {

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


class SpriteId {
public:
    SpriteId(size_t id) _id(id) { }
    explicit operator size_t() { return _id; }

private:
    size_t _id;
};

class FontId {
};

class Sprite {
public:
    void animation(AnimationId animationId);
    void update(double delta);

private:
    void setFrame(size_t frameIndex);

    sf::Sprite _sprite;
    Metronome _metronome;
    schema::Animation* _animation = nullptr;
    size_t _frameIndex = 0;
};

class Font {
private:
    sf::Font _font;
};

void preloadResources();
Sprite load(SpriteId spriteId);
Font load(FontId fontId);

} // namespace ge
