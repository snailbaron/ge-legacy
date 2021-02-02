#pragma once

#include "ge/thing/entity.hpp"

#include <deque>

namespace ge {

class EntityPool {
public:
    Entity createEntity();
    void killEntity(Entity entity);

private:
    std::deque<Entity> _freeEntities;
    Entity::ValueType _nextEntity = 0;
};

} // namespace ge
