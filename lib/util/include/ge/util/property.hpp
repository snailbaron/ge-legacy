#pragma once

#include <functional>
#include <utility>

namespace ge {

template <class T>
class Property {
private:
    Property() {}
    Property(const Property&) = delete;
    Property(Property&&) = delete;

    operator const T&() const
    {
        return _get();
    }

    Property& operator=(T&& value)
    {
        _set(std::forward<T>(value));
    }

private:
    std::function<const T&()> _get = [] { static const T t; return t; };
    std::function<void(T&&)> _set = [] {};
};

} // namespace ge
