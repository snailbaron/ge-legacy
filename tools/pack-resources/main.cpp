#include "ge.hpp"

#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main()
{
    std::string s;
    try {
        auto resourceWriter = ge::ResourceWriter{};

        for (auto record : ge::tyke::Scanner{std::cin}) {
            if (record.type() == "png") {
                resourceWriter.addSprite(
                    record["path"].as<fs::path>(),
                    record["frames"].as<int>());
            } else if (record.type() == "ttf") {
                resourceWriter.addFont(record["path"].as<fs::path>());
            } else {
                std::cerr << "unknown type: [" << record.type() << "]\n";
                return 1;
            }
        }

        resourceWriter.write(std::cout);
    } catch (const ge::str::Scanner::Error& e) {
        std::cerr << e.string << "\n" <<
            std::string(e.position, ' ') << "^" << "\n" <<
            e.message << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
}
