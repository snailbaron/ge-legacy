#pragma once

#include <istream>
#include <map>
#include <optional>
#include <string>
#include <string_view>

namespace ge::tyke {

class Value {
public:
    Value() {}
    explicit Value(std::string text);

    template <class T> T as() const
    {
        assumeNonEmpty();
        return T{*_text};
    }

    template <class T> T optional(T defaultValue = T{}) const
    {
        return _text ? as<T>() : defaultValue;
    }

    friend std::ostream& operator<<(std::ostream& output, const Value& value);

private:
    void assumeNonEmpty() const;

    std::optional<std::string> _text;
};

template <> int Value::as<int>() const;

class Record {
public:
    const std::string& type() const;
    void type(std::string_view t);

    bool contains(std::string_view key) const;

    const Value& operator[](std::string_view key) const;
    void add(std::string_view key, Value value);

    friend std::ostream& operator<<(std::ostream& output, const Record& record);

private:
    static const Value _emptyValue;

    std::string _type;
    std::map<std::string, Value> _parameters;
};

class Scanner {
public:
    class Sentinel {};

    class Iterator {
    public:
        Iterator(std::istream& input);
        Iterator& operator++();

        const Record& operator*() const;

        friend bool operator==(const Iterator& it, Sentinel s);
        friend bool operator==(Sentinel s, const Iterator& it);
        friend bool operator!=(const Iterator& it, Sentinel s);
        friend bool operator!=(Sentinel s, const Iterator& it);

    private:
        void advance();

        std::optional<Record> _record;
        std::istream& _input;
    };

    Scanner(std::istream& input);

    Iterator begin();
    Sentinel end() const;

private:
    std::istream& _input;
};

} // namespace ge::tyke
