#include "ge/client/scene.hpp"

namespace ge {

bool Scene::visible() const
{
    return _visible;
}

void Scene::visible(bool value)
{
    _visible = value;
}

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
    if (!_visible) {
        return;
    }

    target.setView(_view);
    for (const auto& actor : _actors) {
        actor.draw(target);
    }
}

} // namespace ge
