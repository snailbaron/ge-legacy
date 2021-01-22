#include "ge/resources/resources.hpp"

namespace ge {

Frame::Frame(const schema::Frame* frame)
    : _frame(frame)
{ }

int Frame::x() const
{
    return _frame->x();
}

int Frame::y() const
{
    return _frame->y();
}

SpriteData::SpriteData(const schema::Sprite* sprite)
    : _sprite(sprite)
{ }

size_t SpriteData::sheetIndex() const
{
    return 0;
}

int SpriteData::frameMs() const
{
    return _sprite->frame_ms();
}

int SpriteData::width() const
{
    return _sprite->width();
}

int SpriteData::height() const
{
    return _sprite->height();
}

size_t SpriteData::frameCount() const
{
    return _sprite->frames()->size();
}

Frame SpriteData::frame(size_t index) const
{
    return Frame(_sprite->frames()->Get(index));
}

Resources::Resources(const std::filesystem::path& resourceFilePath)
    : _data(resourceFilePath)
    , _root(schema::GetResources(_data.content().data()))
{ }

size_t Resources::sheetCount() const
{
    return _root->sheet()->size() > 0 ? 1 : 0;
}

std::span<const std::byte> Resources::sheet(size_t /*index*/) const
{
    return std::as_bytes(
        std::span{_root->sheet()->data(), _root->sheet()->size()});
}

size_t Resources::spriteCount() const
{
    return _root->sprites()->size();
}

SpriteData Resources::sprite(size_t index) const
{
    return SpriteData{_root->sprites()->Get(index)};
}

size_t Resources::fontCount() const
{
    return _root->fonts()->size();
}

std::span<const std::byte> Resources::font(size_t index) const
{
    auto font = _root->fonts()->Get(index);
    return std::as_bytes(
        std::span{font->bytes()->data(), font->bytes()->size()});
}

} // namespace ge
