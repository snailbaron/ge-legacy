#include "gef/writer.hpp"

#include "ge/error.hpp"
#include "ge/util.hpp"

#include <SFML/Graphics.hpp>

#include <fstream>

namespace fs = std::filesystem;

namespace gef {

size_t Writer::addSprite(
    const fs::path& path, int frameCount, int frameMs)
{
    _spriteFilesInfo.push_back({
        .path = path,
        .frameCount = frameCount,
        .frameMs = frameMs
    });
    return _spriteFilesInfo.size() - 1;
}

size_t Writer::addFont(const fs::path& path)
{
    auto fontBytes = ge::slurp(path);
    auto fontBytesVector = _builder.CreateVector<unsigned char>(fontBytes);
    auto fontOffset = schema::CreateFont(_builder, fontBytesVector);
    _fonts.push_back(fontOffset);
    return _fonts.size() - 1;
}

void Writer::write(std::ostream& output)
{
    std::vector<unsigned char> sheetBytes;
    if (!_spriteFilesInfo.empty()) {
        std::vector<sf::Image> sprites;
        int maxWidth = 0;
        int totalHeight = 0;
        for (const auto& info : _spriteFilesInfo) {
            sf::Image sprite;
            sprite.loadFromFile(info.path);
            GE_ASSERT(sprite.getSize().x % info.frameCount == 0);

            maxWidth = std::max<int>(maxWidth, sprite.getSize().x);
            totalHeight += sprite.getSize().y;

            sprites.push_back(std::move(sprite));
        }

        sf::Image sheet;
        sheet.create(maxWidth, totalHeight, sf::Color{255, 0, 255, 100});
        int y = 0;
        for (size_t i = 0; i < sprites.size(); i++) {
            auto& sprite = sprites.at(i);
            const auto& info = _spriteFilesInfo.at(i);

            sheet.copy(sprite, 0, y);

            auto frameWidth = sprite.getSize().x / info.frameCount;

            std::vector<schema::Frame> frames;
            for (unsigned x = 0; x < sprite.getSize().x; x += frameWidth) {
                frames.emplace_back(x, y);
            }
            auto framesVector = _builder.CreateVectorOfStructs(frames);

            auto spriteBuilder = schema::SpriteBuilder{_builder};
            spriteBuilder.add_frame_ms(info.frameMs);
            spriteBuilder.add_width(frameWidth);
            spriteBuilder.add_height(sprite.getSize().y);
            spriteBuilder.add_frames(framesVector);
            _sprites.push_back(spriteBuilder.Finish());

            y += sprite.getSize().y;

            sprite = sf::Image{};
        }

        // Convert to PNG format. Seemingly no way to do it without saving to file.
        auto sheetFilePath = fs::temp_directory_path() / "sheet.png";
        sheet.saveToFile(sheetFilePath);
        sheet = sf::Image{};
        sheetBytes = ge::slurp(sheetFilePath);
        fs::remove(sheetFilePath);
    }

    auto sheetBytesVector = _builder.CreateVector(sheetBytes);
    auto spritesVector = _builder.CreateVector(_sprites);
    auto fontsVector = _builder.CreateVector(_fonts);

    auto resources = schema::CreateResources(
        _builder, sheetBytesVector, spritesVector, fontsVector);
    _builder.Finish(resources);

    output.write(
        reinterpret_cast<const char*>(_builder.GetBufferPointer()),
        _builder.GetSize());
}

void Writer::write(const fs::path& outputFilePath)
{
    auto output = std::ofstream{outputFilePath, std::ios::binary};
    output.exceptions(std::ios::badbit | std::ios::failbit);
    write(output);
}

} // namespace gef
