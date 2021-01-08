#include "ge/client.hpp"

#include "conversion.hpp"

#include <algorithm>
#include <iterator>
#include <utility>

namespace ge {

namespace {

const sf::Color defaultBackground {50, 50, 50};

} // namespace

void Client::create()
{
    auto contextSettings = sf::ContextSettings{};
    contextSettings.antialiasingLevel = 8;
    _window.create(
        sf::VideoMode::getDesktopMode(), config.windowTitle, sf::Style::None, contextSettings);
    _window.setPosition({0, 0});
    _window.setKeyRepeatEnabled(false);
}

bool Client::isAlive() const
{
    return _window.isOpen();
}

float Client::heightToWidthRatio() const
{
    auto size = _window.getSize();
    return 1.f * size.y / size.x;
}

void Client::processInput()
{
    auto event = sf::Event{};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window.close();
            continue;
        }

        if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
            _window.close();
            continue;
        }

        if (event.type == sf::Event::KeyPressed) {
            auto key = fromSfmlKey(event.key.code);
            if (auto it = _onKeyDown.find(key); it != _onKeyDown.end()) {
                for (const auto& handler : it->second) {
                    handler();
                }
            }
        }
        if (event.type == sf::Event::KeyReleased) {
            auto key = fromSfmlKey(event.key.code);
            if (auto it = _onKeyUp.find(key); it != _onKeyUp.end()) {
                for (const auto& handler : it->second) {
                    handler();
                }
            }
        }
    }
}

void Client::update(double /*delta*/)
{
}

void Client::display()
{
    _window.clear(defaultBackground);
    scene.draw(_window);
    _window.display();
}

void Client::onKeyDownUp(
    Key key, std::function<void()> onKeyDown, std::function<void()> onKeyUp)
{
    _onKeyDown[key].push_back(std::move(onKeyDown));
    _onKeyUp[key].push_back(std::move(onKeyUp));
}

} // namespace ge
