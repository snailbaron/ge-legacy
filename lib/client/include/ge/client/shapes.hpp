#pragma once

#include "ge/client/color.hpp"
#include "ge/client/scene.hpp"

#include <SFML/Graphics.hpp>

namespace ge {

class Circle : public Actor {
public:
    float x() const;
    void x(float xvalue);

    float y() const;
    void y(float yvalue);

    float radius() const;
    void radius(float rvalue);

    Color color() const;
    void color(const Color& color);

    int pointCount() const;
    void pointCount(int value);

    virtual void draw(sf::RenderTarget& target) const override;

private:
    sf::CircleShape _shape;
};

} // namespace ge
