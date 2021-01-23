#include "gef/storage.hpp"

namespace gef {

Sprite::Frame::Frame(const schema::Frame* frame)
    : _frame(frame)
{ }

int Sprite::Frame::x() const
{
    return _frame->x();
}

int Sprite::Frame::y() const
{
    return _frame->y();
}

Sprite::Sprite(const schema::Sprite* sprite)
    : _sprite(sprite)
{
    std::cerr << "loaded sprite (gef data):" <<
        " width=" << _sprite->width() <<
        " height=" << _sprite->height();
    for (unsigned i = 0; i < _sprite->frames()->size(); i++) {
        std::cerr << " (" << _sprite->frames()->Get(i)->x() << ", " <<
            _sprite->frames()->Get(i)->y() << ")";
    }
    std::cerr << "\n";
}

size_t Sprite::sheetIndex() const
{
    return 0;
}

int Sprite::frameMs() const
{
    return _sprite->frame_ms();
}

int Sprite::width() const
{
    return _sprite->width();
}

int Sprite::height() const
{
    return _sprite->height();
}

size_t Sprite::frameCount() const
{
    return _sprite->frames()->size();
}

Sprite::Frame Sprite::frame(size_t index) const
{
    return Sprite::Frame(_sprite->frames()->Get(index));
}

Storage::Storage(const std::filesystem::path& resourceFilePath)
    : _data(resourceFilePath)
    , _root(schema::GetResources(_data.content().data()))
{ }

std::span<const std::byte> Storage::sheet() const
{
    return std::as_bytes(
        std::span{_root->sheet()->data(), _root->sheet()->size()});
}

size_t Storage::spriteCount() const
{
    return _root->sprites()->size();
}

Sprite Storage::sprite(size_t index) const
{
    std::cerr << "loading sprite " << index << "\n";
    return Sprite{_root->sprites()->Get(index)};
}

size_t Storage::fontCount() const
{
    return _root->fonts()->size();
}

std::span<const std::byte> Storage::font(size_t index) const
{
    auto bytes = _root->fonts()->Get(index)->bytes();
    return std::as_bytes(std::span{bytes->data(), bytes->size()});
}

} // namespace gef
