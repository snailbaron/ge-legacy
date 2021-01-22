#include "ge.hpp"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

bool isValidResourceId(std::string_view s)
{
    if (s.empty()) {
        return false;
    }
    if (!std::isalpha(s[0])) {
        return false;
    }
    for (size_t i = 1; i < s.size(); i++) {
        if (!std::isalnum(s[i])) {
            return false;
        }
    }
    return true;
}

void pack(
    const fs::path& resourceDescriptionFilePath,
    const fs::path& outputDataFilePath,
    const fs::path& outputHeaderPath)
{
    struct FontIdNames {
        ge::FontId id;
        std::string name;
    };

    struct SpriteIdNames {
        ge::SpriteId id;
        std::string name;
    };

    std::vector<FontIdNames> fontIdNames;
    std::vector<SpriteIdNames> spriteIdNames;

    auto resourceWriter = ge::ResourceWriter{};

    auto input = std::ifstream{resourceDescriptionFilePath};
    for (auto record : ge::tyke::Scanner{input}) {
        std::cerr << "record: " << record << "\n";

        auto name = record["id"].optional<std::string>();
        if (name.empty()) {
            name = record["path"].as<fs::path>().stem().string();
        }
        if (!isValidResourceId(name)) {
            throw ge::Exception{} << "invalid name '" << name <<
                "' in resource " << record;
        }

        if (record.type() == "sprite") {
            auto id = resourceWriter.addSprite(
                record["path"].as<fs::path>(),
                record["frames"].optional<int>(1));
            spriteIdNames.push_back({.id = id, .name = name});
        } else if (record.type() == "font") {
            auto id = resourceWriter.addFont(record["path"].as<fs::path>());
            fontIdNames.push_back({.id = id, .name = name});
        } else {
            throw ge::Exception{} << "unknown resource type: [" <<
                record.type() << "]\n";
        }
    }

    std::cerr << "writing output file to " << outputDataFilePath << "\n";
    resourceWriter.write(outputDataFilePath);

    std::cerr << "writing header to " << outputHeaderPath << "\n";
    auto header = std::ofstream{outputHeaderPath};
    if (!header.is_open()) {
        throw ge::Exception{} <<
            "cannot create output header: " << outputHeaderPath;
    }
    header <<
        "#pragma once\n" <<
        "\n" <<
        "#include <ge/resources/ids.hpp>\n" <<
        "\n" <<
        "namespace re {\n" <<
        "\n" <<
        "namespace sprite {\n" <<
        "\n";
    for (const auto& p : spriteIdNames) {
        header << "constexpr auto " << p.name <<
            " = ge::SpriteId{" << p.id << "};\n";
    }
    header <<
        "\n" <<
        "} // namespace sprite\n" <<
        "\n" <<
        "namespace font {\n" <<
        "\n";
    for (const auto& p : fontIdNames) {
        header << "constexpr auto " << p.name <<
            " = ge::FontId{" << p.id << "};\n";
    }
    header <<
        "\n" <<
        "} // namespace font\n" <<
        "\n" <<
        "} // namespace re\n";
}

void unpack(const fs::path& dataFilePath)
{
    std::cerr << "creating resources\n";
    auto resources = ge::Resources{dataFilePath};

    for (size_t i = 0; i < resources.sheetCount(); i++) {
        std::cerr << "unpacking sheet " << i << "\n";
        auto sheet = resources.sheet(i);
        sf::Image image;
        image.loadFromMemory(sheet.data(), sheet.size());
        image.saveToFile("sheet_" + std::to_string(i) + ".png");
    }

    for (size_t i = 0; i < resources.spriteCount(); i++) {
        std::cerr << "unpacking sprite " << i << "\n";
        auto s = resources.sprite(i);
        std::cout << s.width() << " x " << s.height() << ", " <<
            s.frameCount() << " frames on sheet " << s.sheetIndex() <<
            " at " << s.frameMs() << " ms:";
        for (size_t j = 0; j < s.frameCount(); j++) {
            auto frame = s.frame(j);
            std::cout << " (" << frame.x() << ", " << frame.y() << ")";
        }
        std::cout << "\n";
    }

    for (size_t i = 0; i < resources.fontCount(); i++) {
        std::cerr << "unpacking font " << i << "\n";
        auto font = resources.font(i);
        ge::write(font, "font_" + std::to_string(i) + ".ttf");
    }
}

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++) {
        std::cerr << "arg: '" << argv[i] << "'\n";
    }

    auto printUsage = [] {
        std::cout << R"_(
Usage:
    re pack RESOURCE_DESCRIPTION_FILE OUTPUT_DATA_FILE OUTPUT_HEADER
    re unpack RESOURCE_DATA_FILE
        )_";
    };

    try {
        if (argc < 2) {
            printUsage();
            return 1;
        }
        auto command = std::string{argv[1]};

        if (command == "pack") {
            if (argc != 5) {
                printUsage();
                return 1;
            }
            auto resourceDescriptionFilePath = fs::path{argv[2]};
            auto outputDataFilePath = fs::path{argv[3]};
            auto outputHeaderPath = fs::path{argv[4]};
            pack(resourceDescriptionFilePath, outputDataFilePath, outputHeaderPath);
        } else if (command == "unpack") {
            if (argc != 3) {
                printUsage();
                return 1;
            }
            auto resourceDataFilePath = fs::path{argv[2]};
            unpack(resourceDataFilePath);
        } else {
            std::cerr << "unknown command: '" << command << "'\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}
