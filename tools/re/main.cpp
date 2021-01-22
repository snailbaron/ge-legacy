#include "ge.hpp"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

void pack(const fs::path& resourceDescriptionFilePath)
{
    auto resourceWriter = ge::ResourceWriter{};

    auto input = std::ifstream{resourceDescriptionFilePath};
    for (auto record : ge::tyke::Scanner{input}) {
        if (record.type() == "png") {
            resourceWriter.addSprite(
                record["path"].as<fs::path>(),
                record["frames"].optional<int>(1));
        } else if (record.type() == "ttf") {
            resourceWriter.addFont(record["path"].as<fs::path>());
        } else {
            throw ge::Exception{} << "unknown resource type: [" <<
                record.type() << "]\n";
        }
    }

    resourceWriter.write(std::cout);
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
    try {
        if (argc != 3) {
            std::cout <<
                "Usage:\n" <<
                "    re pack RESOURCE_DESCRIPTION_FILE\n" <<
                "    re unpack RESOURCE_DATA_FILE\n";
            return 0;
        }

        auto command = std::string{argv[1]};
        auto filePath = fs::path{argv[2]};
        if (command == "pack") {
            pack(filePath);
        } else if (command == "unpack") {
            unpack(filePath);
        } else {
            std::cerr << "unknown command: '" << command << "'\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}
