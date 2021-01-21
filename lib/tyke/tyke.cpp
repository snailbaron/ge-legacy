#include "ge/tyke.hpp"

#include "ge/error.hpp"
#include "ge/str.hpp"

#include <cctype>

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

Value::operator const std::string&() const
{
    return _text;
}

template <> int Value::as<int>() const
{
    return std::stoi(_text);
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
    auto it = _parameters.find(std::string{key});
    if (it == _parameters.end()) {
        throw Exception{} << "no key: '" << key << "'";
    }
    return it->second;
}

void Record::add(std::string_view key, Value value)
{
    auto [it, inserted] =
        _parameters.insert({std::string{key}, std::move(value)});
    if (!inserted) {
        throw Exception{} << "key already set: '" << key << "'";
    }
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
    for (std::string line; std::getline(_input, line); ) {
        _record = parseResourceDescription(line);
        if (_record) {
            break;
        }
    }
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
