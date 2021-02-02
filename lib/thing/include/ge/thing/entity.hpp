#pragma once

#include <cstdint>

namespace ge {

class Entity {
public:
    using ValueType = uint32_t;

    explicit Entity(ValueType id) : _id(id) {}

    operator ValueType() const { return _id; }

    friend bool operator==(Entity lhs, Entity rhs)
    {
        return lhs._id == rhs._id;
    }

    friend bool operator!=(Entity lhs, Entity rhs)
    {
        return lhs._id != rhs._id;
    }

    friend bool operator<(Entity lhs, Entity rhs)
    {
        return lhs._id < rhs._id;
    }

    friend bool operator>(Entity lhs, Entity rhs)
    {
        return lhs._id > rhs._id;
    }

    friend bool operator<=(Entity lhs, Entity rhs)
    {
        return lhs._id <= rhs._id;
    }

    friend bool operator>=(Entity lhs, Entity rhs)
    {
        return lhs._id >= rhs._id;
    }

private:
    ValueType _id;
};

} // namespace ge
