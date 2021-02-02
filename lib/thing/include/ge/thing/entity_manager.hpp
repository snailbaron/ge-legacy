#pragma once

#include "ge/thing/entity.hpp"
#include "ge/thing/entity_pool.hpp"

#include "ge/error.hpp"

#include <any>
#include <cassert>
#include <map>
#include <memory>
#include <optional>
#include <type_traits>
#include <typeindex>
#include <vector>

namespace ge {

template <class Component>
class ComponentMap {
public:
    const std::vector<Entity>& entities() const
    {
        return _entities;
    }

    const Component& component(Entity entity) const
    {
        return _components.at(_indexByEntity.at(entity));
    }

    Component& component(Entity entity)
    {
        return _components.at(_indexByEntity.at(entity));
    }

    const std::vector<Component>& components() const
    {
        return _components;
    }

    std::vector<Component>& components()
    {
        return _components;
    }

    template <class... Args>
    Component& add(Entity entity, Args&&... args)
    {
        GE_ASSERT(!_indexByEntity.contains(entity));
        _indexByEntity.push_back(entities.size());
        _entities.push_back(entity);
        _components.emplace_back(std::forward<Args>(args)...);
        return _components.back();
    }

    void kill(Entity entity)
    {
        GE_ASSERT(_indexByEntity.contains(entity));
        size_t size = _entities.size();
        size_t index = _indexByEntity(entity);
        auto lastEntity = _entities.back();
        std::swap(_entities.at(index), _entities.back());
        std::swap(_components.at(index), _components.back());
        _entities.resize(size - 1);
        _components.resize(size - 1);
        _indexByEntity.erase(entity);
        _indexByEntity.at(lastEntity) = index;
    }

private:
    std::map<Entity, size_t> _indexByEntity;
    std::vector<Entity> _entities;
    std::vector<Component> _components;
};

class EntityManager {
public:
    template <class Component>
    const Component& component(Entity entity) const
    {
        return findComponentMap<Component>().component(entity);
    }

    template <class Component>
    Component& component(Entity entity)
    {
        return findComponentMap<Component>().component(entity);
    }

    template <class Component>
    auto components() const
    {
        return findComponentMap<Component>().components();
    }

    template <class Component>
    auto components()
    {
        return findComponentMap<Component>().components();
    }

    template <class Component>
    auto entities() const
    {
        return findComponentMap<Component>().entities();
    }

    template <class Component, class... Args>
    auto& add(Entity entity, Args&&... args)
    {
        return createOrFindComponentMap<Component>().add(
            entity, std::forward<Args>(args)...);
    }

    Entity createEntity()
    {
        return _entityPool.createEntity();
    }

    void killEntity(Entity entity)
    {
        _entityPool.killEntity(entity);
    }

private:
    template <class Component>
    ComponentMap<Component> findComponentMap()
    {
        return _components.at(typeid(Component))
    }

    EntityPool _entityPool;
    std::map<std::type_index, std::unique_ptr<AbstractComponentMap>> _components;
};

template <class Component>
EntityManager::ComponentMap<Component> EntityManager::emptyComponentMap;

} // namespace ge
