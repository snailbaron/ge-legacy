#include <lyra/lyra.hpp>

#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    fs::path asepritePath;
    std::vector<fs::path> asepriteFilePaths;

    auto cli
        = lyra::opt(asepritePath, "aseprite path")
            ["--aseprite-path"]
            ("Path to Aseprite executable")
        | lyra::opt(asepriteFilePaths, "aseprite file paths")
            ["--file-paths"]
            ("Paths to input files in Aseprite format")
        ;

    if (auto result = cli.parse({argc, argv}); !result) {
        std::cerr << "command line error: " << result.errorMessage() << "\n";
        return EXIT_FAILURE;
    }

    auto jsonPath = fs::temp_directory_path() / "meta.json";
    auto command =
        asepritePath.string() +
        " --batch" +
        " --data " + jsonPath.string() +
        " --filename-format {title}:{tag}:{tagframe}" +
        " --format json-array" +
        " --sheet-type packed";
    std::system(command.c_str());
}
