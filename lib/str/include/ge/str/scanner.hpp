#pragma once

#include "ge/error.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace ge::str {

template <class F>
bool matches(char c, const F& f)
{
    return f(c);
}

inline bool matches(char c, char etalon)
{
    return c == etalon;
}

class Scanner {
public:
    Scanner(std::string_view s)
        : _s(s)
    { }

    bool done() const
    {
        return _i >= _s.length();
    }

    template <class... Ts>
    void skip(Ts&&... matchers)
    {
        while (_i < _s.length() && (matches(_s[_i], matchers) || ...)) {
            _i++;
        }
    }

    void expect(char c)
    {
        if (_i >= _s.length() || _s[_i] != c) {
            error("expected: '" + std::string{c} + "'");
        }
        _i++;
    }

    template <class... Ts>
    std::string_view captureUntil(const Ts&... matchers)
    {
        auto startIndex = _i;
        while (_i < _s.length() && !(matches(_s[_i], matchers) || ...)) {
            _i++;
        }
        return _s.substr(startIndex, _i - startIndex);
    }

private:
    void error(std::string_view message)
    {
        throw Exception{} <<
            _s << "\n" <<
            std::string(_i, ' ') << "^" << "\n" <<
            message << "\n";
    }

    std::string_view _s;
    size_t _i = 0;
};

} // namespace ge::str
