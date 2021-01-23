#include "ge/client/resources.hpp"

namespace fs = std::filesystem;

namespace ge {

namespace {

std::optional<Resources> _resources;

const Resources& resources()
{
    if (!_resources) {
        loadResources();
    }
    return *_resources;
}

fs::path findDefaultResourcesPath()
{
    // TODO: search in several well-known directories
    return currentExecutableDirectory() / "resources.data";
}

} // namespace

Resources::Resources(const std::filesystem::path& dataFilePath)
    : _storage(dataFilePath)
    , _texture(_storage.sheet())
{ }

std::shared_ptr<Sprite> Resources::load(SpriteId id) const
{
    auto sprite = std::make_shared<Sprite>(_texture);
    auto data = _storage.sprite(id);
    std::cerr << "Resources::load: sprite data frame count: " << data.frameCount() << "\n";

    sprite->frameSeconds(data.frameMs() / 1000.f);
    for (size_t i = 0; i < data.frameCount(); i++) {
        auto frame = data.frame(i);
        std::cerr << "adding frame\n";
        sprite->addFrame(frame.x(), frame.y(), data.width(), data.height());
    }
    return sprite;
}

void loadResources()
{
    _resources.emplace(findDefaultResourcesPath());
}

void unloadResources()
{
    _resources.reset();
}

std::shared_ptr<Sprite> load(SpriteId id)
{
    return resources().load(id);
}

} // namespace ge
