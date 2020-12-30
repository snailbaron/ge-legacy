#pragma once

#include <exception>
#include <sstream>
#include <stdexcept>
#include <string>

#define GE_ASSERT(CONDITION)                                                \
    do {                                                                    \
        if (!(CONDITION)) {                                                 \
            throw std::runtime_error("ASSERT failed: '" #CONDITION "'");    \
        }                                                                   \
    } while (false)

#define GE_ASSERT_MESSAGE(CONDITION, MESSAGE)                   \
    do {                                                        \
        if (!(CONDITION)) {                                     \
            throw std::runtime_error(                           \
                "ASSERT failed: '" #CONDITION "': " MESSAGE);   \
        }                                                       \
    } while (false)

namespace ge {

class Exception : public std::exception {
public:
    template <class T>
    Exception& operator<<(const T& value)
    {
        auto stream = std::ostringstream{std::move(_string)};
        stream << value;
        _string = stream.str();
        return *this;
    }

    const char* what() const noexcept override
    {
        return _string.c_str();
    }

private:
    std::string _string;
};

} // namespace
