#include <ge/time/frame-timer.hpp>

namespace ge::time {

FrameTimer::FrameTimer(int fps)
    : _delta(1.f / fps)
    , _frameDuration(std::chrono::duration_cast<Clock::duration>(
        std::chrono::duration<float>(_delta)))
    , _lastFrame(Clock::now())
    , _currentTime(_lastFrame)
{ }

float FrameTimer::delta() const
{
    return _delta;
}

int FrameTimer::operator()()
{
    _currentTime = Clock::now();
    if (_lastFrame + _frameDuration > _currentTime) {
        return 0;
    }

    int framesPassed = (_currentTime - _lastFrame) / _frameDuration;
    _lastFrame += framesPassed * _frameDuration;
    return framesPassed;
}

void FrameTimer::relax()
{
    std::this_thread::sleep_until(_lastFrame + _frameDuration);
}

} // namespace ge::time
