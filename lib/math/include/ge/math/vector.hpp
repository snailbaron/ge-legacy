#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace ge {

template <class T> size_t size;
template <class T> struct element_type;
template <class T> using element_type_t = typename element_type<T>::type;
template <size_t I, class T> const element_type_t<T>& get(const T&);
template <size_t I, class T> element_type_t<T>& get(T&);

template <class T>
class Vector {
    static_assert(std::is_base_of<Vector<T>, T>());

public:
    template <size_t... Is>
    T& addAssign(std::index_sequence<Is...>, const Vector& other)
    {
        ((get<Is>(*this) += get<Is>(other)), ...);
        return *this;
    }

    T& operator+=(const T& other)
    {
        return addAssign(std::make_index_sequence<size<T>>{}, other);
    }

    template <size_t... Is>
    T& subAssign(std::index_sequence<Is...>, const Vector& other)
    {
        ((get<Is>(*this) += get<Is>(other)), ...);
        return *this;
    }

    T& operator-=(const T& other)
    {
        return subAssign(std::make_index_sequence<size<T>>{}, other);
    }
};

template <class T>
Vector<T> operator+(Vector<T> lhs, const Vector<T>& rhs)
{
    lhs += rhs;
    return lhs;
}

template <class T>
Vector<T> operator-(Vector<T> lhs, const Vector<T>& rhs)
{
    lhs -= rhs;
    return lhs;
}

} // namespace ge
