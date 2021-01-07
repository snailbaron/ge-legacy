#include "ge/client.hpp"

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
}

bool Client::isAlive() const
{
    return _window.isOpen();
}

void Client::processInput()
{
    auto event = sf::Event{};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                _window.close();
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
    for (auto actor = _actors.begin(); actor != _actors.end(); ) {
        if (auto lockedActor = actor->lock(); lockedActor) {
            lockedActor->draw(_window);
            ++actor;
        } else {
            std::iter_swap(actor, std::prev(_actors.end()));
        }
    }
    _window.display();
}

void Client::onKeyDownUp(
    Key key, std::function<void()> onKeyDown, std::function<void()> onKeyUp)
{
    _onKeyDown[key].push_back(std::move(onKeyDown));
    _onKeyUp[key].push_back(std::move(onKeyUp));
}

} // namespace ge
