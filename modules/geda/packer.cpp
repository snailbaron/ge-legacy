#include <ge/packer.hpp>

#include "schema_generated.h"

#include <ge/error.hpp>
#include <ge/util.hpp>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <utility>

namespace ge {

namespace fs = std::filesystem;

namespace {

ResourceType detectResourceType(const fs::path& resourceFilePath)
{
    auto ext = resourceFilePath.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [] (unsigned char c) {
        return std::tolower(c);
    });

    if (ext == ".png") {
        return ResourceType::PNG;
    } else if (ext == ".ttf") {
        return ResourceType::TTF;
    } else {
        throw ge::Exception{} << "cannot detect type of resource at " <<
            resourceFilePath;
    }
}

std::string nameFromPath(const fs::path& path)
{
    return path.stem().string();
}

} // namespace

ResourcePacker& ResourcePacker::add(
    const fs::path& resourceFilePath, ResourceType resourceType)
{
    switch (resourceType) {
        case ResourceType::PNG:
            _pngPaths.push_back(resourceFilePath);
            break;

        case ResourceType::TTF:
            _ttfPaths.push_back(resourceFilePath);
            break;
    }

    return *this;
}

ResourcePacker& ResourcePacker::add(const fs::path& resourceFilePath)
{
    add(resourceFilePath, detectResourceType(resourceFilePath));
    return *this;
}

void ResourcePacker::pack(
    const std::filesystem::path& targetPath,
    const std::filesystem::path& asepritePath)
{
    auto builder = flatbuffers::FlatBufferBuilder{};
    auto dataBuilder = schema::DataBuilder{builder};

    std::vector<flatbuffers::Offset<schema::Font>> fonts;
    for (const auto& ttfPath : _ttfPaths) {
        auto name = nameFromPath(ttfPath);
        auto content = slurp(ttfPath);

        auto fontBuilder = schema::FontBuilder{builder};
        fontBuilder.add_name(builder.CreateString(name));
        fontBuilder.add_data(builder.CreateVector(content));
        fonts.push_back(fontBuilder.Finish());
    }
    dataBuilder.add_fonts(builder.CreateVector(fonts));

    for (const auto& pngPath : _pngPaths) {
        auto name = nameFromPath(pngPath);
        auto content = slurp(pngPath);

        auto graphicsBuilder = schema::GraphicsBuilder{builder};
    }

    auto output = std::ofstream{targetPath, std::ios::binary};
    output.exceptions(std::ios::badbit | std::ios::failbit);
    output.write(
        reinterpret_cast<const char*>(builder.GetBufferPointer()),
        builder.GetSize());
}

} // namespace ge
