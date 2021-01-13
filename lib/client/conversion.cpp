#include "conversion.hpp"

namespace ge {

namespace {

struct KeyPair {
    Key key;
    sf::Keyboard::Key sfmlKey;
};

constexpr auto _keyPairs = std::to_array<KeyPair>({
    { Key::Unknown, sf::Keyboard::Key::Unknown },
    { Key::A,       sf::Keyboard::Key::A       },
    { Key::B,       sf::Keyboard::Key::B       },
    { Key::C,       sf::Keyboard::Key::C       },
    { Key::D,       sf::Keyboard::Key::D       },
    { Key::E,       sf::Keyboard::Key::E       },
    { Key::F,       sf::Keyboard::Key::F       },
    { Key::G,       sf::Keyboard::Key::G       },
    { Key::H,       sf::Keyboard::Key::H       },
    { Key::I,       sf::Keyboard::Key::I       },
    { Key::J,       sf::Keyboard::Key::J       },
    { Key::K,       sf::Keyboard::Key::K       },
    { Key::L,       sf::Keyboard::Key::L       },
    { Key::M,       sf::Keyboard::Key::M       },
    { Key::N,       sf::Keyboard::Key::N       },
    { Key::O,       sf::Keyboard::Key::O       },
    { Key::P,       sf::Keyboard::Key::P       },
    { Key::Q,       sf::Keyboard::Key::Q       },
    { Key::R,       sf::Keyboard::Key::R       },
    { Key::S,       sf::Keyboard::Key::S       },
    { Key::T,       sf::Keyboard::Key::T       },
    { Key::U,       sf::Keyboard::Key::U       },
    { Key::V,       sf::Keyboard::Key::V       },
    { Key::W,       sf::Keyboard::Key::W       },
    { Key::X,       sf::Keyboard::Key::X       },
    { Key::Y,       sf::Keyboard::Key::Y       },
    { Key::Z,       sf::Keyboard::Key::Z       },
    { Key::F1,      sf::Keyboard::Key::F1      },
    { Key::F2,      sf::Keyboard::Key::F2      },
    { Key::F3,      sf::Keyboard::Key::F3      },
    { Key::F4,      sf::Keyboard::Key::F4      },
    { Key::F5,      sf::Keyboard::Key::F5      },
    { Key::F6,      sf::Keyboard::Key::F6      },
    { Key::F7,      sf::Keyboard::Key::F7      },
    { Key::F8,      sf::Keyboard::Key::F8      },
    { Key::F9,      sf::Keyboard::Key::F9      },
    { Key::F10,     sf::Keyboard::Key::F10     },
    { Key::F11,     sf::Keyboard::Key::F11     },
    { Key::F12,     sf::Keyboard::Key::F12     },
    { Key::Space,   sf::Keyboard::Key::Space   },
    { Key::Left,    sf::Keyboard::Key::Left    },
    { Key::Right,   sf::Keyboard::Key::Right   },
    { Key::Up,      sf::Keyboard::Key::Up      },
    { Key::Down,    sf::Keyboard::Key::Down    },
});

} // namespace

sf::Keyboard::Key toSfmlKey(Key key)
{
    for (const auto& p : _keyPairs) {
        if (key == p.key) {
            return p.sfmlKey;
        }
    }
    return sf::Keyboard::Key::Unknown;
}

Key fromSfmlKey(sf::Keyboard::Key sfmlKey)
{
    for (const auto& p : _keyPairs) {
        if (sfmlKey == p.sfmlKey) {
            return p.key;
        }
    }
    return Key::Unknown;
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
