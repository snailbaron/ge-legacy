#include "ge/client.hpp"

#include "conversion.hpp"

#include <algorithm>
#include <iterator>
#include <utility>

#include <iostream>

namespace ge {

namespace {

const sf::Color defaultBackground {50, 50, 50};

} // namespace

void Client::create()
{
    auto contextSettings = sf::ContextSettings{};
    contextSettings.antialiasingLevel = 8;
    _window.create(
        sf::VideoMode::getDesktopMode(), windowTitle, sf::Style::Fullscreen, contextSettings);
    _window.setPosition({0, 0});
    _window.setKeyRepeatEnabled(false);
}

bool Client::isAlive() const
{
    return _window.isOpen();
}

void Client::kill()
{
    _window.close();
}

int Client::width() const
{
    return _window.getSize().x;
}

int Client::height() const
{
    return _window.getSize().y;
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
                event.key.alt && event.key.code == sf::Keyboard::F4) {
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

void Client::update(double delta)
{
    for (auto& scene : _scenes) {
        scene.update(delta);
    }
}

void Client::display()
{
    if (!_window.isOpen()) {
        return;
    }

    _window.clear(defaultBackground);
    for (auto& scene : _scenes) {
        scene.draw(_window);
    }
    _window.display();
}

void Client::onKeyDown(Key key, std::function<void()> onKeyDown)
{
    _onKeyDown[key].push_back(std::move(onKeyDown));
}

void Client::onKeyUp(Key key, std::function<void()> onKeyUp)
{
    _onKeyUp[key].push_back(std::move(onKeyUp));
}

void Client::onKeyDownUp(
    Key key, std::function<void()> onKeyDown, std::function<void()> onKeyUp)
{
    this->onKeyDown(key, std::move(onKeyDown));
    this->onKeyUp(key, std::move(onKeyUp));
}

Scene& Client::scene(int index)
{
    return _scenes.at(index);
}

void Client::sceneNumber(int number)
{
    _scenes.resize(number);
}

} // namespace ge
