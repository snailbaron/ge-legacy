#pragma once

#include "ge/client/color.hpp"
#include "ge/client/keyboard.hpp"

#include <SFML/Graphics.hpp>

namespace ge {

sf::Keyboard::Key toSfmlKey(Key key);
Key fromSfmlKey(sf::Keyboard::Key sfmlKey);

sf::Color toSfmlColor(const Color& color);
Color fromSfmlColor(const sf::Color& color);

} // namespace ge
