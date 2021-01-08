#pragma once

#include "ge/util.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

namespace ge {

class Actor {
public:
    virtual ~Actor() {}
    virtual void update(double /*delta*/) {}
    virtual void draw(sf::RenderTarget& target) const = 0;
};

class Scene {
public:
    template <class T, class... Args>
    std::shared_ptr<T> spawn(Args&&... args)
    {
        return _actors.spawn<T>(std::forward<Args>(args)...);
    }

    void view(float x, float y, float w, float h);
    void update(double delta);
    void draw(sf::RenderTarget& target) const;

private:
    Registry<Actor> _actors;
    sf::View _view {{0, 0, 1, 1}};
};

} // namespace ge
