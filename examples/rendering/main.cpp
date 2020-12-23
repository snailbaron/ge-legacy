#include <ge.hpp>

constexpr auto FPS = 60;

int main()
{
    auto renderer = ge::Renderer{};

    auto treeSprite = load(ge::sprite::Tree);
    auto anotherTreeSprite = load(ge::sprite::Tree);
    auto font = load(ge::font::OpenSans);

    auto timer = ge::FrameTimer{FPS};
    for (;;) {
        if (int framesPassed = timer(); framesPassed > 0)
        {
            auto delta = timer.delta() * framesPassed;
            treeSprite.update(delta);
            anotherTreeSprite.update(delta);

            renderer.clear();
            renderer.draw(treeSprite);
            renderer.draw(anotherTreeSprite);
            renderer.draw(font);
            renderer.display();
        }
        timer.relax();
    }
}
