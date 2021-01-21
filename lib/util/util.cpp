#include <ge/util.hpp>

#include <fstream>

namespace fs = std::filesystem;

namespace ge {

std::vector<unsigned char> slurp(const fs::path& path)
{
    auto input = std::ifstream{path, std::ios::binary | std::ios::ate};
    input.exceptions(std::ios::badbit | std::ios::failbit);
    std::vector<unsigned char> buffer;
    buffer.resize(input.tellg());
    input.seekg(0, std::ios::beg);
    input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
    return buffer;
}

} // namespace ge
