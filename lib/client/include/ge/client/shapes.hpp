#pragma once

#include "ge/client/client.hpp"

#include <SFML/Graphics.hpp>

namespace ge {

class Circle : public Actor {
public:
    Circle();

    float x() const;
    void x(float xvalue);

    float y();
    void y(float yvalue);

    virtual void draw(sf::RenderTarget& target) const override;

private:
    sf::CircleShape _shape;
    sf::Transformable _transformable;
};

} // namespace ge
