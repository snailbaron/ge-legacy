#pragma once

#include <SFML/Graphics.hpp>

namespace ge {

class Actor {
public:
    virtual ~Actor() {}
    virtual void update(double /*delta*/) {}
    virtual void draw(sf::RenderTarget& target) const = 0;
};

} // namespace ge
