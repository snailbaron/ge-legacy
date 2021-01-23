#pragma once

#include "ge/client/sprite.hpp"

#include "gef.hpp"

#include <filesystem>

namespace ge {

class SpriteId {
public:
    explicit constexpr SpriteId(size_t id) : _id(id) {}
    constexpr operator size_t() const { return _id; }

private:
    size_t _id = 0;
};

class FontId {
public:
    explicit constexpr FontId(size_t id) : _id(id) {}
    constexpr operator size_t() const { return _id; }

private:
    size_t _id = 0;
};

class Resources {
public:
    Resources(const std::filesystem::path& dataFilePath);

    std::shared_ptr<Sprite> load(SpriteId id) const;

private:
    gef::Storage _storage;
    Texture _texture;
};

void loadResources();
void unloadResources();

std::shared_ptr<Sprite> load(SpriteId id);

} // namespace ge
