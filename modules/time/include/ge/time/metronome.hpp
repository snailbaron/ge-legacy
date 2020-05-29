#pragma once

namespace ge::time {

class Metronome {
public:
    static Metronome atFps(int fps);
    static Metronome byFrame(float frameDuration);

    int operator()(float delta);

private:
    Metronome(float frameDuration);

    float _frameDuration;
    float _offset = 0.f;
};

} // namespace ge::time
