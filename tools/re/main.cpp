#include "ge.hpp"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

void pack(
    const fs::path& resourceDescriptionFilePath, const fs::path& outputFilePath)
{
    auto resourceWriter = ge::ResourceWriter{};

    auto input = std::ifstream{resourceDescriptionFilePath};
    for (auto record : ge::tyke::Scanner{input}) {
        std::cerr << "record: " << record << "\n";
        if (record.type() == "sprite") {
            resourceWriter.addSprite(
                record["path"].as<fs::path>(),
                record["frames"].optional<int>(1));
        } else if (record.type() == "font") {
            resourceWriter.addFont(record["path"].as<fs::path>());
        } else {
            throw ge::Exception{} << "unknown resource type: [" <<
                record.type() << "]\n";
        }
    }

    std::cerr << "writing output file to " << outputFilePath << "\n";
    resourceWriter.write(outputFilePath);
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
    re pack RESOURCE_DESCRIPTION_FILE OUTPUT_FILE
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
            if (argc != 4) {
                printUsage();
                return 1;
            }
            auto resourceDescriptionFilePath = fs::path{argv[2]};
            auto outputFilePath = fs::path{argv[3]};
            pack(resourceDescriptionFilePath, outputFilePath);
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
