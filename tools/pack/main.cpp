#include <ge/packer.hpp>

#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    auto packer = ge::ResourcePacker{};
    for (int i = 1; i < argc; i++) {
        packer.add(fs::path{argv[i]});
    }
    packer.pack("data.ge");
}
