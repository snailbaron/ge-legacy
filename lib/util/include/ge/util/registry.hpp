#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include <iostream>

namespace ge {

template <class T>
class Registry {
public:
    class EndSentinel {};

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
        {
            advanceTillValid();
        }

        ElementRef operator*()
        {
            return *_it->lock();
        }

        CommonIterator& operator++()
        {
            ++_it;
            advanceTillValid();
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

        friend bool operator==(const CommonIterator& it, EndSentinel)
        {
            return it._it == it._ptrs.end();
        }

        friend bool operator!=(const CommonIterator& it, EndSentinel end)
        {
            return !(it == end);
        }

        friend bool operator==(EndSentinel end, const CommonIterator& it)
        {
            return it == end;
        }

        friend bool operator!=(EndSentinel end, const CommonIterator& it)
        {
            return it != end;
        }

    private:
        void advanceTillValid()
        {
            if constexpr (Const) {
                while (_it != _ptrs.end() && _it->expired()) {
                    ++_it;
                }
            } else {
                auto end = _ptrs.end();
                while (_it != end && _it->expired()) {
                    //std::cerr << "moving end\n";
                    --end;
                    std::iter_swap(_it, end);
                }

                if (_it == end) {
                    //std::cerr << "it was end\n";
                    _ptrs.erase(end, _ptrs.end());
                    _it = _ptrs.end();
                } else {
                    //std::cerr << "it was not end\n";
                    _ptrs.erase(end, _ptrs.end());
                }
            }
        }

        ContainerRef _ptrs;
        ContainerIterator _it;
    };

    using Iterator = CommonIterator<false>;
    using ConstIterator = CommonIterator<true>;

    // TODO(bug): move iterator to a valid position after begin()

    ConstIterator begin() const
    {
        return {_ptrs, _ptrs.begin()};
    }

    Iterator begin()
    {
        return {_ptrs, _ptrs.begin()};
    }

    EndSentinel end() const
    {
        return {};
    }

    template <class U> requires std::is_convertible_v<U*, T*>
    void attach(const std::shared_ptr<U>& p)
    {
        _ptrs.push_back(p);
    }

    template <class Target, class... Args>
    std::shared_ptr<Target> spawn(Args&&... args)
    {
        static_assert(std::is_convertible<Target*, T*>());
        auto p = std::make_shared<Target>(std::forward<Args>(args)...);
        _ptrs.push_back(p);
        return p;
    }

private:
    std::vector<std::weak_ptr<T>> _ptrs;
};

} // namespace ge
