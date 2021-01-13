#pragma once

#include "ge/client/scene.hpp"

#include <SFML/Graphics.hpp>

#include <cstddef>
#include <span>

namespace ge {

class Texture {
public:
    Texture() {}
    Texture(std::span<const std::byte> data);

    void load(std::span<const std::byte> data);
    void unload();

private:
    sf::Texture _sfmlTexture;

    friend class Sprite;
};

class Sprite : public Actor {
public:
    Sprite(const Texture& texture);

    void addFrame(int x, int y, int w, int h);

    float x() const;
    void x(float xvalue);

    float y() const;
    void y(float yvalue);

    float frameSeconds() const;
    void frameSeconds(float value);

    float pixelSize() const;
    void pixelSize(float value);

    float rotation() const;
    void rotation(float value);

    void update(double delta) override;
    void draw(sf::RenderTarget& target) const override;

private:
    sf::Sprite _sfmlSprite;
    std::vector<sf::IntRect> _frames;
    float _frameSeconds = 1.f;
    float _pixelSize = 1.f;
    float _rotation = 0.f;
    size_t _currentFrame = 0;
    float _secondsRes = 0.f;
};

} // namespace ge
