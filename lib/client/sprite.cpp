#include "ge/client/sprite.hpp"

#include <cmath>

namespace ge {

Texture::Texture(std::span<const std::byte> data)
{
    load(data);
}

void Texture::load(std::span<const std::byte> data)
{
    _sfmlTexture.loadFromMemory(data.data(), data.size());
}

void Texture::unload()
{
    _sfmlTexture = sf::Texture{};
}

Sprite::Sprite(const Texture& texture)
    : _sfmlSprite(texture._sfmlTexture)
{ }

void Sprite::addFrame(int x, int y, int w, int h)
{
    _frames.emplace_back(x, y, w, h);
    _sfmlSprite.setOrigin(w / 2, h / 2);
    _sfmlSprite.setTextureRect(_frames.at(_currentFrame));
}

float Sprite::x() const
{
    return _sfmlSprite.getPosition().x;
}

void Sprite::x(float xvalue)
{
    _sfmlSprite.setPosition(xvalue, y());
}

float Sprite::y() const
{
    return _sfmlSprite.getPosition().y;
}

void Sprite::y(float yvalue)
{
    _sfmlSprite.setPosition(x(), yvalue);
}

float Sprite::frameSeconds() const
{
    return _frameSeconds;
}

void Sprite::frameSeconds(float value)
{
    _frameSeconds = value;
}

float Sprite::pixelSize() const
{
    return _pixelSize;
}

void Sprite::pixelSize(float value)
{
    _pixelSize = value;
    _sfmlSprite.setScale(_pixelSize, -_pixelSize);
}

float Sprite::rotation() const
{
    return _rotation;
}

void Sprite::rotation(float value)
{
    _rotation = value;
    _sfmlSprite.setRotation(value);
}

void Sprite::update(double delta)
{
    _secondsRes =
        std::fmod(_secondsRes + delta, _frameSeconds * _frames.size());
    auto frame = static_cast<size_t>(_secondsRes / _frameSeconds);
    if (frame != _currentFrame) {
        _currentFrame = frame;
        _sfmlSprite.setTextureRect(_frames.at(_currentFrame));
    }
}

void Sprite::draw(sf::RenderTarget& target) const
{
    target.draw(_sfmlSprite);
}

} // namespace ge
