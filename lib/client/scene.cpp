#include "ge/client/scene.hpp"

namespace ge {

void Scene::view(float x, float y, float w, float h)
{
    _view.reset({x, y + h, w, -h});
}

void Scene::update(double delta)
{
    for (auto& actor : _actors) {
        actor.update(delta);
    }
}

void Scene::draw(sf::RenderTarget& target) const
{
    target.setView(_view);
    for (const auto& actor : _actors) {
        actor.draw(target);
    }
}

} // namespace ge
