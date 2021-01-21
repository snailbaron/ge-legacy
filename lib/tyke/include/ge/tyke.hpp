#pragma once

#include <istream>
#include <map>
#include <optional>
#include <string>
#include <string_view>

namespace ge::tyke {

class Value {
public:
    explicit Value(std::string text = "");
    operator const std::string&() const;

    template <class T> T as() const
    {
        return T{_text};
    }

private:
    std::string _text;
};

template <> int Value::as<int>() const;

class Record {
public:
    const std::string& type() const;
    void type(std::string_view t);

    bool contains(std::string_view key) const;

    const Value& operator[](std::string_view key) const;
    void add(std::string_view key, Value value);

private:
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
