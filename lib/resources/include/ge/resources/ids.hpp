#pragma once

#include <cstddef>

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

} // namespace ge
