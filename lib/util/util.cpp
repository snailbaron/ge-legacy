#include <ge/util.hpp>

#include <ge/error.hpp>

#include <fstream>

namespace fs = std::filesystem;

namespace ge {

std::vector<unsigned char> slurp(const fs::path& path)
{
    auto input = std::ifstream{path, std::ios::binary | std::ios::ate};
    if (!input.is_open()) {
        throw Exception{} << "slurp: cannot open file: " << path;
    }
    std::vector<unsigned char> buffer;
    buffer.resize(input.tellg());
    input.seekg(0, std::ios::beg);
    input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
    return buffer;
}

void write(const std::vector<unsigned char>& data, const fs::path& path)
{
    auto output = std::ofstream{path, std::ios::binary};
    output.exceptions(std::ios::badbit | std::ios::failbit);
    output.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void write(const std::span<const std::byte>& bytes, const fs::path& path)
{
    auto output = std::ofstream{path, std::ios::binary};
    output.exceptions(std::ios::badbit | std::ios::failbit);
    output.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

} // namespace ge
