#include "ge/client/shapes.hpp"

namespace ge {

Circle::Circle()
{
    _shape.setRadius(100.f);
    _shape.setFillColor(sf::Color::White);
    _shape.setOutlineColor(sf::Color::Green);
    _shape.setPosition(10.f, 10.f);
    _shape.setPointCount(64);
}

float Circle::x() const
{
    return _transformable.getPosition().x;
}

void Circle::x(float xvalue)
{
    _transformable.setPosition(xvalue, y());
}

float Circle::y()
{
    return _transformable.getPosition().y;
}

void Circle::y(float yvalue)
{
    _transformable.setPosition(x(), yvalue);
}

void Circle::draw(sf::RenderTarget& target) const
{
    target.draw(_shape, _transformable.getTransform());
}

} // namespace ge
