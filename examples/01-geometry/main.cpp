#include <ge.hpp>

int main()
{
    auto client = ge::Client{};

    const int fps = 60;
    auto timer = ge::FrameTimer{fps};

    while (client.isAlive()) {
        client.processInput();

        if (auto framesPassed = timer(); framesPassed > 0) {
            client.update(framesPassed * timer.delta());
        }

        timer.relax();
    }
}
