#pragma once

#include "widget.hpp"
#include "root.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace ge {

class Layout {
public:
    void onMouseMove(int x, int y)
    {
        if (_focusedWidget && /* _focusedWidget contains x, y */) {
            return;
        }

        if (_focusedWidget) {
            _focusedWidget->onFocusLost();
            _focusedWidget = nullptr;
        }

        const auto globalPosition = Vector{x, y};
        auto offset = Vector{};
        Widget* widget = &_root;
        for (;;) {
            auto it = std::find_if(
                widget->children().begin(),
                widget->children().end(),
                [&globalPosition, &offset] (const auto& child) {
                    return child->contains(globalPosition - offset);
                });
            if (it == widget->children().end()) {
                break;
            }
            offset += (*it)->position();
            widget = (*it).get();
        }
    }

private:
    Root _root;
    Vector _focusedWidgetOffset;
    Widget* _focusedWidget = nullptr;
    Widget* _pressedWidget = nullptr;
};

} // namespace ge
