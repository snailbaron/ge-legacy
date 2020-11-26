#include <ge/time/metronome.hpp>

#include <cmath>

namespace ge {

Metronome Metronome::atFps(int fps)
{
    return {1.f / fps};
}

Metronome Metronome::byFrame(float frameDuration)
{
    return {frameDuration};
}

int Metronome::operator()(float delta)
{
    _offset += delta;
    int ticks = static_cast<int>(_offset / _frameDuration);
    _offset = std::fmod(_offset, _frameDuration);
    return ticks;
}

Metronome::Metronome(float frameDuration)
    : _frameDuration(frameDuration)
{ }

} // namespace ge::time
