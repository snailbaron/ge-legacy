#pragma once

#include "ge/util.hpp"

#include "schema_generated.h"

#include <array>
#include <cstddef>
#include <filesystem>
#include <span>

namespace ge {

class Frame {
public:
    explicit Frame(const schema::Frame* frame);

    int x() const;
    int y() const;

private:
    const schema::Frame* _frame = nullptr;
};

class SpriteData {
public:
    explicit SpriteData(const schema::Sprite* sprite);

    size_t sheetIndex() const;
    int frameMs() const;
    int width() const;
    int height() const;
    size_t frameCount() const;
    Frame frame(size_t index) const;

private:
    const schema::Sprite* _sprite = nullptr;
};

class Resources {
public:
    explicit Resources(const std::filesystem::path& resourceFilePath);

    size_t sheetCount() const;
    std::span<const std::byte> sheet(size_t index) const;

    size_t spriteCount() const;
    SpriteData sprite(size_t index) const;

    size_t fontCount() const;
    std::span<const std::byte> font(size_t index) const;

private:
    ReadOnlyFileMap _data;
    const schema::Resources* _root = nullptr;
};

} // namespace ge
