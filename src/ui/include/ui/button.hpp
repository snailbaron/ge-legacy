#pragma once

#include "backend.hpp"
#include "vector.hpp"
#include "widget.hpp"

#include <functional>
#include <string>
#include <utility>

namespace ge {

class Button : public Widget {
public:
    Button()
    {
        _background = roundedRectangleGeometry();
    }

    Button& position(const Vector& position)
    {
        _position = position;
        return *this;
    }

    Button& size(const Vector& size)
    {
        _size = size;
        return *this;
    }

    Button& font(Font font)
    {
        _font = std::move(font);
        return *this;
    }

    Button& text(std::string text)
    {
        _text.setString(std::move(text));
        return *this;
    }

    Button& action(std::function<void()> action)
    {
        _action = std::move(action);
        return *this;
    }

    void draw(Renderer& renderer, const Vector& offset) const override
    {
        auto p = offset + _position;
        renderer.renderGeometry(_background, _color, p);
        renderer.renderText(_text, _textColor, p);
    }

    void onFocus() override
    {
        _color = _focusedColor;
    }

    void onFocusLost() override
    {
        _color = _normalColor;
    }

    void onPress() override
    {
        _color = _pressedColor;
    }

private:
    static Color _normalColor = Color{0, 100, 0};
    static Color _focusedColor = Color{0, 200, 0};
    static Color _pressedColor = Color{200, 200, 0};
    static Color _textColor = Color{0, 0, 0};

    Geometry _background;
    Text _text;
    Font _font;
    Color _color = _normalColor;

    Vector _position;
    Vector _size;

    std::function<void()> _action;
};

} // namespace ge
