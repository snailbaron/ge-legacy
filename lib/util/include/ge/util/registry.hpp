#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace ge {

template <class T>
class Registry {
public:
    template <bool Const>
    class CommonIterator {
    public:
        using Container = std::vector<std::weak_ptr<T>>;
        using ContainerIterator = std::conditional_t<
            Const,
            typename Container::const_iterator,
            typename Container::iterator>;
        using ContainerRef =
            std::conditional_t<Const, const Container&, Container&>;
        using ElementRef = std::conditional_t<Const, const T&, T&>;

        CommonIterator(ContainerRef ptrs, ContainerIterator it)
            : _ptrs(ptrs)
            , _it(it)
        { }

        ElementRef operator*()
        {
            return *_it->lock();
        }

        CommonIterator& operator++()
        {
            if constexpr (Const) {
                do {
                    ++_it;
                } while (_it != _ptrs.end() && _it->expired());
            } else {
                auto end = _ptrs.end();
                ++_it;
                while (_it != end && _it->expired()) {
                    --end;
                    std::iter_swap(_it, end);
                }

                if (_it == end) {
                    _ptrs.erase(end, _ptrs.end());
                    _it = _ptrs.end();
                } else {
                    _ptrs.erase(end, _ptrs.end());
                }
            }
            return *this;
        }

        CommonIterator operator++(int)
        {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        friend bool operator==(
            const CommonIterator& lhs, const CommonIterator& rhs)
        {
            return lhs._it == rhs._it;
        }

        friend bool operator!=(
            const CommonIterator& lhs, const CommonIterator& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        ContainerRef _ptrs;
        ContainerIterator _it;
    };

    using Iterator = CommonIterator<false>;
    using ConstIterator = CommonIterator<true>;

    ConstIterator begin() const
    {
        return {_ptrs, _ptrs.begin()};
    }

    Iterator begin()
    {
        return {_ptrs, _ptrs.begin()};
    }

    ConstIterator end() const
    {
        return {_ptrs, _ptrs.end()};
    }

    Iterator end()
    {
        return {_ptrs, _ptrs.end()};
    }

    template <class Target, class... Args>
    std::shared_ptr<Target> spawn(Args&&... args)
    {
        static_assert(std::is_base_of<T, Target>());
        auto p = std::make_shared<Target>(std::forward<Args>(args)...);
        _ptrs.push_back(p);
        return p;
    }

private:
    std::vector<std::weak_ptr<T>> _ptrs;
};

} // namespace ge
