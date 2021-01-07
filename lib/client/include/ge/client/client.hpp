#pragma once

#include "ge/client/keyboard.hpp"

#include <SFML/Graphics.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ge {

class Actor {
public:
    virtual ~Actor() {}
    virtual void draw(sf::RenderTarget& target) const = 0;
};

class Client {
public:
    struct Config {
        std::string windowTitle = "ge";
    };

    bool isAlive() const;

    void create();
    void processInput();
    void update(double delta);
    void display();

    void onKeyDownUp(
        Key key, std::function<void()> onKeyDown, std::function<void()> onKeyUp);

    template <class T, class... Args>
    std::shared_ptr<T> spawn(Args&&... args)
    {
        static_assert(std::is_base_of<Actor, T>());
        auto p = std::make_shared<T>(std::forward<Args>(args)...);
        _actors.push_back(p);
        return p;
    }

    Config config;

private:
    sf::RenderWindow _window;    
    std::map<Key, std::vector<std::function<void()>>> _onKeyUp;
    std::map<Key, std::vector<std::function<void()>>> _onKeyDown;
    std::vector<std::weak_ptr<Actor>> _actors;
};

} // namespace ge
