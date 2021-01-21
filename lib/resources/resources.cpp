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

} // namespace ge
