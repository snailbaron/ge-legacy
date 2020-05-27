#pragma once

#include "backend.hpp"
#include "vector.hpp"

#include <memory>
#include <vector>

namespace ge {

class Widget {
public:
    virtual ~Widget() {}

    const Vector& position() const
    {
        return _position;
    }

    const std::vector<std::unique_ptr<Widget>>& children() const
    {
        return _children;
    }

    virtual bool contains() { return false; }
    virtual void update(float delta) {}
    virtual void draw(Renderer& renderer, const Vector& offset) const {}

    virtual void onFocus() {}
    virtual void onFocusLost() {}
    virtual void onPress() {}
    virtual void onRelease() {}
    virtual void onDrag() {}

private:
    Vector _position;
    std::vector<std::unique_ptr<Widget>> _children;
};

} // namespace ge
