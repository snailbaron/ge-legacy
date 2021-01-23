#pragma once

#include "ge/client/actor.hpp"
#include "ge/client/resources.hpp"

#include "ge/util.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

namespace ge {

class Scene {
public:
    void attach(const std::shared_ptr<Actor>& actor);

    template <class T, class... Args>
    std::shared_ptr<T> spawn(Args&&... args)
    {
        return _actors.spawn<T>(std::forward<Args>(args)...);
    }

    std::shared_ptr<Sprite> spawn(SpriteId spriteId);

    bool visible() const;
    void visible(bool value);

    void view(float x, float y, float w, float h);
    void update(double delta);
    void draw(sf::RenderTarget& target) const;

private:
    Registry<Actor> _actors;
    sf::View _view {{0, 0, 1, 1}};
    bool _visible = true;
};

} // namespace ge
