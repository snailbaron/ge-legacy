#include "ge/resources/resource_writer.hpp"

#include "ge/error.hpp"
#include "ge/util.hpp"

#include <SFML/Graphics.hpp>

namespace fs = std::filesystem;

namespace ge {

SpriteId ResourceWriter::addSprite(
    const fs::path& path, int frameCount, int frameMs)
{
    _spriteFilesInfo.push_back({
        .path = path,
        .frameCount = frameCount,
        .frameMs = frameMs
    });
    return SpriteId{_spriteFilesInfo.size() - 1};
}

FontId ResourceWriter::addFont(const fs::path& path)
{
    auto fontBytes = slurp(path);
    auto fontOffset =
        schema::CreateFont(_builder, _builder.CreateVector(fontBytes));
    _fonts.push_back(fontOffset);
    return FontId{_fonts.size() - 1};
}

void ResourceWriter::write(std::ostream& output)
{
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
    sheet.create(maxWidth, totalHeight);
    int y = 0;
    for (size_t i = 0; i < sprites.size(); i++) {
        auto& sprite = sprites.at(i);
        const auto& info = _spriteFilesInfo.at(i);

        sheet.copy(sprite, 0, y);
        y += sprite.getSize().y;

        auto frameWidth = sprite.getSize().x / info.frameCount;
        auto spriteBuilder = schema::SpriteBuilder{_builder};
        spriteBuilder.add_frame_ms(info.frameMs);
        spriteBuilder.add_width(frameWidth);
        spriteBuilder.add_height(sprite.getSize().y);
        std::vector<schema::Frame> frames;
        for (unsigned x = 0; x < sprite.getSize().x; x += frameWidth) {
            frames.emplace_back(x, y);
        }
        spriteBuilder.add_frames(_builder.CreateVectorOfStructs(frames));
        _sprites.push_back(spriteBuilder.Finish());

        sprite = sf::Image{};
    }

    // Convert to PNG format. Seemingly no way to do it without saving to file.
    auto sheetFilePath = fs::temp_directory_path() / "sheet.png";
    sheet.saveToFile(sheetFilePath);
    sheet = sf::Image{};

    auto sheetBytes = slurp(sheetFilePath);
    fs::remove(sheetFilePath);

    auto resourcesBuilder = schema::ResourcesBuilder{_builder};
    resourcesBuilder.add_sheet(_builder.CreateVector(sheetBytes));
    resourcesBuilder.add_sprites(_builder.CreateVector(_sprites));
    resourcesBuilder.add_fonts(_builder.CreateVector(_fonts));
    _builder.Finish(resourcesBuilder.Finish());

    output.write(
        reinterpret_cast<const char*>(_builder.GetBufferPointer()),
        _builder.GetSize());
}

} // namespace ge
