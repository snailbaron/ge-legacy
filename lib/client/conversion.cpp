#include "conversion.hpp"

namespace ge {

sf::Keyboard::Key toSfmlKey(Key key)
{
    switch (key) {
        case Key::Unknown: return sf::Keyboard::Unknown;
        case Key::A: return sf::Keyboard::A;
        case Key::B: return sf::Keyboard::B;
        case Key::C: return sf::Keyboard::C;
        case Key::D: return sf::Keyboard::D;
        case Key::E: return sf::Keyboard::E;
        case Key::F: return sf::Keyboard::F;
        case Key::G: return sf::Keyboard::G;
        case Key::H: return sf::Keyboard::H;
        case Key::I: return sf::Keyboard::I;
        case Key::J: return sf::Keyboard::J;
        case Key::K: return sf::Keyboard::K;
        case Key::L: return sf::Keyboard::L;
        case Key::M: return sf::Keyboard::M;
        case Key::N: return sf::Keyboard::N;
        case Key::O: return sf::Keyboard::O;
        case Key::P: return sf::Keyboard::P;
        case Key::Q: return sf::Keyboard::Q;
        case Key::R: return sf::Keyboard::R;
        case Key::S: return sf::Keyboard::S;
        case Key::T: return sf::Keyboard::T;
        case Key::U: return sf::Keyboard::U;
        case Key::V: return sf::Keyboard::V;
        case Key::W: return sf::Keyboard::W;
        case Key::X: return sf::Keyboard::X;
        case Key::Y: return sf::Keyboard::Y;
        case Key::Z: return sf::Keyboard::Z;
        case Key::Space: return sf::Keyboard::Space;
        case Key::Left: return sf::Keyboard::Left;
        case Key::Up: return sf::Keyboard::Up;
        case Key::Right: return sf::Keyboard::Right;
        case Key::Down: return sf::Keyboard::Down;
    }
    return sf::Keyboard::Unknown;
}

Key fromSfmlKey(sf::Keyboard::Key sfmlKey)
{
    switch (sfmlKey) {
        case sf::Keyboard::A: return Key::A;
        case sf::Keyboard::B: return Key::B;
        case sf::Keyboard::C: return Key::C;
        case sf::Keyboard::D: return Key::D;
        case sf::Keyboard::E: return Key::E;
        case sf::Keyboard::F: return Key::F;
        case sf::Keyboard::G: return Key::G;
        case sf::Keyboard::H: return Key::H;
        case sf::Keyboard::I: return Key::I;
        case sf::Keyboard::J: return Key::J;
        case sf::Keyboard::K: return Key::K;
        case sf::Keyboard::L: return Key::L;
        case sf::Keyboard::M: return Key::M;
        case sf::Keyboard::N: return Key::N;
        case sf::Keyboard::O: return Key::O;
        case sf::Keyboard::P: return Key::P;
        case sf::Keyboard::Q: return Key::Q;
        case sf::Keyboard::R: return Key::R;
        case sf::Keyboard::S: return Key::S;
        case sf::Keyboard::T: return Key::T;
        case sf::Keyboard::U: return Key::U;
        case sf::Keyboard::V: return Key::V;
        case sf::Keyboard::W: return Key::W;
        case sf::Keyboard::X: return Key::X;
        case sf::Keyboard::Y: return Key::Y;
        case sf::Keyboard::Z: return Key::Z;
        case sf::Keyboard::Space: return Key::Space;
        case sf::Keyboard::Left: return Key::Left;
        case sf::Keyboard::Up: return Key::Up;
        case sf::Keyboard::Right: return Key::Right;
        case sf::Keyboard::Down: return Key::Down;
        default: return Key::Unknown;
    }
}

sf::Color toSfmlColor(const Color& color)
{
    return {color.r, color.g, color.b, color.a};
}

Color fromSfmlColor(const sf::Color& color)
{
    return {color.r, color.g, color.b, color.a};
}

} // namespace ge
