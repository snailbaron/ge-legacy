#pragma once

#include "ge/client/keyboard.hpp"
#include "ge/client/scene.hpp"

#include <SFML/Graphics.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ge {

class Client {
public:
    struct Config {
        std::string windowTitle = "ge";
    };

    bool isAlive() const;
    void kill();
    int width() const;
    int height() const;

    void create();
    void processInput();
    void update(double delta);
    void display();

    void onKeyDownUp(
        Key key, std::function<void()> onKeyDown, std::function<void()> onKeyUp);

    Config config;
    Scene scene;

private:
    sf::RenderWindow _window;    
    std::map<Key, std::vector<std::function<void()>>> _onKeyUp;
    std::map<Key, std::vector<std::function<void()>>> _onKeyDown;
};

} // namespace ge
