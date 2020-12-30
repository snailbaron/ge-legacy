#include "ge/resources/data.hpp"
#include "schema_generated.h"

#include <fstream>

namespace fs = std::filesystem;

namespace ge {

namespace {

ge::Sprite Resources::load(SpriteId spriteId) const
{
    auto animation = _root->animations(spriteId);
}



class ResourceHolder {
public:
    ResourceHolder(const std::filesystem::path& dataFilePath);

private:
    std::vector<unsigned char> _buffer;
    const schema::Resources* _data;
    sf::Texture _texture;
};

const schema::Resources* resources()
{
    static ResourceHolder holder;
    return holder.data;
}

} // namespace

void Sprite::animation(AnimationId animationId)
{
    _animation = resources().data.animation(animationId);
    _metronome.reset();
    setFrame(0);
}

void Sprite::update(double delta)
{
    if (animation) {
        if (auto offset = _metronome(); offset > 0) {
            setFrame((_frameIndex + offset) % _animation->frames_size());
        }
    }
}

void Sprite::setFrame(size_t frameIndex)
{
    _frameIndex = frameIndex;
    auto frame = _animation->frames(_frameIndex);
    _sprite.setTextureRect({frame.x, frame.y, frame.w, frame.h});
}

Resources::Resources(const std::filesystem::path& dataFilePath)
{
    auto input = std::ifstream{dataFilePath, std::ios::binary};
    input.seekg(0, std::ios::end);
    auto inputLength = input.tellg();
    input.seekg(0, std::ios::beg);
    _buffer.resize(inputLength);
    input.read(_buffer.data(), inputLength);
    _data = GetResources(_buffer);

    _texture.loadFromMemory(_data->sheet, _data->sheet_size());
}

Sprite load(SpriteId spriteId)
{
}

} // namespace ge
