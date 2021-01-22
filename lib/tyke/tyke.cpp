#include "ge/tyke.hpp"

#include "ge/error.hpp"
#include "ge/str.hpp"

#include <cctype>
#include <iostream>

namespace ge::tyke {

namespace {

std::optional<Record> parseResourceDescription(std::string_view s)
{
    Record result;

    if (auto p = s.find('#'); p != std::string_view::npos) {
        s.remove_suffix(s.size() - p);
    }

    auto scanner = str::Scanner{s};

    scanner.skip(isspace);
    if (scanner.done()) {
        return std::nullopt;
    }
    scanner.expect('[');
    result.type(scanner.captureUntil(']'));
    scanner.expect(']');
    scanner.skip(isspace);

    while (!scanner.done()) {
        auto key = scanner.captureUntil(isspace, '=');
        scanner.skip(isspace);
        scanner.expect('=');
        scanner.skip(isspace);
        auto valueText = scanner.captureUntil(';');
        GE_ASSERT(!result.contains(valueText));
        result.add(key, Value{std::string{valueText}});
        scanner.expect(';');
        scanner.skip(isspace);
    }

    return result;
}

} // namespace

Value::Value(std::string text)
    : _text(text)
{ }

template <> int Value::as<int>() const
{
    assumeNonEmpty();
    return std::stoi(*_text);
}

void Value::assumeNonEmpty() const
{
    if (!_text) {
        throw Exception{} << "ge::tyke::Value is empty";
    }
}

const Value Record::_emptyValue {};

std::ostream& operator<<(std::ostream& output, const Value& value)
{
    value.assumeNonEmpty();
    return output << *value._text;
}

const std::string& Record::type() const
{
    return _type;
}

void Record::type(std::string_view t)
{
    _type = t;
}

bool Record::contains(std::string_view key) const
{
    return _parameters.contains(std::string{key});
}

const Value& Record::operator[](std::string_view key) const
{
    if (auto it = _parameters.find(std::string{key}); it != _parameters.end()) {
        return it->second;
    } else {
        return _emptyValue;
    }
}

void Record::add(std::string_view key, Value value)
{
    auto [it, inserted] =
        _parameters.insert({std::string{key}, std::move(value)});
    if (!inserted) {
        throw Exception{} << "key already set: '" << key << "'";
    }
}

std::ostream& operator<<(std::ostream& output, const Record& record)
{
    output << "[" << record._type << "]";
    for (const auto& [key, value] : record._parameters) {
        output << " " << key << " = " << value << ";";
    }
    return output;
}

Scanner::Iterator::Iterator(std::istream& input)
    : _input(input)
{
    advance();
}

Scanner::Iterator& Scanner::Iterator::operator++()
{
    advance();
    return *this;
}

const Record& Scanner::Iterator::operator*() const
{
    return _record.value();
}

bool operator==(const Scanner::Iterator& it, Scanner::Sentinel)
{
    return !it._record;
}

bool operator==(Scanner::Sentinel s, const Scanner::Iterator& it)
{
    return (it == s);
}

bool operator!=(const Scanner::Iterator& it, Scanner::Sentinel s)
{
    return !(it == s);
}

bool operator!=(Scanner::Sentinel s, const Scanner::Iterator& it)
{
    return !(s == it);
}

void Scanner::Iterator::advance()
{
    std::optional<Record> nextRecord;
    for (std::string line; std::getline(_input, line); ) {
        nextRecord = parseResourceDescription(line);
        if (nextRecord) {
            break;
        }
    }
    _record = std::move(nextRecord);
}

Scanner::Scanner(std::istream& input)
    : _input(input)
{ }

Scanner::Iterator Scanner::begin()
{
    return {_input};
}

Scanner::Sentinel Scanner::end() const
{
    return {};
}

} // namespace ge::tyke
