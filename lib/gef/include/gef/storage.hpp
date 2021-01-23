#pragma once

#include "ge/util.hpp"

#include "schema_generated.h"

#include <array>
#include <cstddef>
#include <filesystem>
#include <span>

namespace gef {

class Sprite {
public:
    class Frame {
    public:
        explicit Frame(const schema::Frame* frame);

        int x() const;
        int y() const;

    private:
        const schema::Frame* _frame = nullptr;
    };

    explicit Sprite(const schema::Sprite* sprite);

    size_t sheetIndex() const;
    int frameMs() const;
    int width() const;
    int height() const;
    size_t frameCount() const;
    Frame frame(size_t index) const;

private:
    const schema::Sprite* _sprite = nullptr;
};

class Storage {
public:
    explicit Storage(const std::filesystem::path& resourceFilePath);

    std::span<const std::byte> sheet() const;

    size_t spriteCount() const;
    Sprite sprite(size_t index) const;

    size_t fontCount() const;
    std::span<const std::byte> font(size_t index) const;

private:
    ge::ReadOnlyFileMap _data;
    const schema::Resources* _root = nullptr;
};

} // namespace gef
