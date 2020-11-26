#pragma once

#include <chrono>

namespace ge {

class FrameTimer {
public:
    explicit FrameTimer(int fps);
    double delta() const;
    int operator()();
    void relax();

private:
    using Clock = std::chrono::high_resolution_clock;

    const double _delta;
    const Clock::duration _frameDuration;
    Clock::time_point _lastFrame;
    Clock::time_point _currentTime;
    Clock::time_point _nextFrame;
};

} // namespace ge::time
