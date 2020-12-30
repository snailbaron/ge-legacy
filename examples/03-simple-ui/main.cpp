#include <ge.hpp>

constexpr auto gameFps = 60;


int main()
{
    auto timer = ge::time::FrameTimer{gameFps};
    for (;;) {
    }
}
