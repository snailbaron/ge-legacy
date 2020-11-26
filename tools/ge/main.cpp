#include <ge/error.hpp>

#include <tclap/CmdLine.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

std::string nameFromPath(const fs::path& path)
{
    return path.stem().string();
}

void packResources(
    const std::vector<fs::path>& asepritePaths,
    const std::vector<fs::path>& pngPaths,
    const std::vector<fs::path>& ttfPaths,
    const fs::path& targetPath,
    const fs::path& tempDirPath,
    const fs::path& asepriteExePath)
{
    auto allAsePaths = asepritePaths;
    for (const auto& pngPath : pngPaths) {
        auto asePath = pngPath;
        asePath.replace_extension(".ase");
        allAsePaths.push_back(asePath);

        auto command = std::ostringstream{};
        command << asepriteExePath << " --batch " << pngPath <<
            " --save-as " << asePath;
        auto commandString = command.str();
        std::system(commandString.c_str());
    }

    const auto jsonPath = tempDirPath / "meta.json";
    auto packCommand = std::ostringstream{};
    packCommand <<
        asepriteExePath <<
        " --batch" <<
        " --data" << jsonPath <<
        " --filename-format {title}:{tag}:{tagframe}" <<
        " --format json-array" <<
        " --sheet-type packed";
}

int pack(int argc, char** argv, lyra::cli_parser& cli)
{
    bool showHelp;
    std::vector<fs::path> asepritePaths;
    std::vector<fs::path> pngPaths;
    std::vector<fs::path> ttfPaths;
    fs::path targetPath = "data.ge";
    fs::path tempDirPath;
    fs::path asepriteExePath;
    cli |= lyra::help(showHelp);
    cli |= lyra::opt(asepritePaths, "ASEPRITE")
        ["--ase"]
        ("path to an aseprite resource");
    cli |= lyra::opt(pngPaths, "PNG")
        ["--png"]
        ("path to a png resource");
    cli |= lyra::opt(ttfPaths, "TTF")
        ["--ttf"]
        ("path to a ttf resource");
    cli |= lyra::opt(targetPath, "TARGET")
        ["-o", "--output"]
        ("path to the output data file");
    cli |= lyra::opt(tempDirPath, "TEMP_DIR")
        ["--temp"]
        ("path to the directory with temporary data files");
    cli |= lyra::opt(asepriteExePath, "ASEPRITE_executable")
        .required()
        ["--aseprite-path"]
        ("path to aseprite executable");

    if (auto result = cli.parse({argc, argv}); !result) {
        std::cerr << "command line error: " << result.errorMessage() << "\n" <<
            cli << "\n";
        return EXIT_FAILURE;
    }

    if (showHelp) {
        std::cout << cli << "\n";
        return EXIT_SUCCESS;
    }

    if (tempDirPath.empty()) {
        tempDirPath = fs::temp_directory_path();
    }

    packResources(
        asepritePaths,
        pngPaths,
        ttfPaths,
        targetPath,
        tempDirPath,
        asepriteExePath);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    static const std::map<
            std::string,
            int(*)(int, char**, lyra::cli_parser&)> commands {
        {"pack", pack},
    };

    std::string command;
    auto cli = lyra::cli_parser{}
        | lyra::arg(command, "command")
            ("command to run")
            .required()
            .choices(
                [] (const std::string& value) -> bool {
                return commands.count(value);
            })
        ;

    if (auto result = cli.parse({argc, argv}); !result) {
        std::cerr << "command line error: " << result.errorMessage() << "\n" <<
            cli << "\n";
        return EXIT_FAILURE;
    }

    if (!command.empty()) {
        return commands.at(command)(argc, argv, cli);
    } else {
        std::cout << cli << "\n";
        return EXIT_SUCCESS;
    }
}
