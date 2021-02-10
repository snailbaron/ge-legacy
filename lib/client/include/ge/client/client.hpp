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
    bool isAlive() const;
    void kill();
    int width() const;
    int height() const;

    void create();
    void processInput();
    void update(double delta);
    void display();

    void onKeyDown(Key key, std::function<void()> onKeyDown);
    void onKeyUp(Key key, std::function<void()> onKeyDown);
    void onKeyDownUp(
        Key key, std::function<void()> onKeyDown, std::function<void()> onKeyUp);

    Scene& scene(int index = 0);
    void sceneNumber(int number);

    std::string windowTitle;

private:
    sf::RenderWindow _window;    
    std::vector<Scene> _scenes = std::vector<Scene>(1);
    std::map<Key, std::vector<std::function<void()>>> _onKeyUp;
    std::map<Key, std::vector<std::function<void()>>> _onKeyDown;
};

} // namespace ge
