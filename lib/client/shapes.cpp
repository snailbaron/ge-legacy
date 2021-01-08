#include "ge/client/shapes.hpp"

#include "conversion.hpp"

namespace ge {

float Circle::x() const
{
    return _shape.getPosition().x;
}

void Circle::x(float xvalue)
{
    _shape.setPosition(xvalue, y());
}

float Circle::y() const
{
    return _shape.getPosition().y;
}

void Circle::y(float yvalue)
{
    _shape.setPosition(x(), yvalue);
}

float Circle::radius() const
{
    return _shape.getRadius();
}

void Circle::radius(float rvalue)
{
    _shape.setRadius(rvalue);
    _shape.setOrigin(rvalue, rvalue);
}

Color Circle::color() const
{
    return fromSfmlColor(_shape.getFillColor());
}

void Circle::color(const Color& color)
{
    _shape.setFillColor(toSfmlColor(color));
}

void Circle::draw(sf::RenderTarget& target) const
{
    target.draw(_shape);
}

int Circle::pointCount() const
{
    return _shape.getPointCount();
}

void Circle::pointCount(int value)
{
    _shape.setPointCount(value);
}

} // namespace ge
