#include "ge/imp.hpp"

#include <arg.hpp>

#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    auto pngPaths = arg::multiOption<fs::path>()
        .help("paths to PNG images");
    arg::parse(argc, argv);

    ge::imp::mergeImages(pngPaths);
}
