#include <ge.hpp>

#include <algorithm>
#include <vector>

struct Point {
    float x;
    float y;
};

struct Game {
    void update(double delta)
    {
        float shipAcceleration = -1.0 * leftPressed + 1.0 * rightPressed;
        shipSpeed += shipAcceleration * delta;
        shipSpeed = std::clamp(shipSpeed, -shipMaxSpeed, shipMaxSpeed);
        shipPosition += shipSpeed * delta;
        shipPosition = std::clamp(shipPosition, 0.f, screenWidth);

        for (auto bullet = bullets.begin(); bullet != bullets.end(); ) {
            bullet->y += bulletSpeed * delta;
            bool hit = false;
            for (auto enemy = enemies.begin(); enemy != enemies.end(); ) {
                auto dx = bullet->x - enemy->x;
                auto dy = bullet->y - enemy->y;
                if (dx * dx + dy * dy <= hitDistance * hitDistance) {
                    hit = true;
                    std::iter_swap(enemy, std::prev(enemies.end()));
                    enemies.resize(enemies.size() - 1);
                } else {
                    ++enemy;
                }
            }

            if (hit) {
                std::iter_swap(bullet, std::prev(bullets.end()));
                bullets.resize(bullets.size() - 1);
            } else {
                ++bullet;
            }
        }

        secondsSinceLastShot += delta;
        if (shotRequested && secondsSinceLastShot >= shotCooldownSeconds) {
            bullets.push_back({shipPosition, 0.f});
        }

        timeToSpawn -= delta;
        while (timeToSpawn <= 0) {
            timeToSpawn += spawnPeriodSeconds;
            // TODO: spawn
        }

        timeToSpeedUpSpawn -= delta;
        while (timeToSpeedUpSpawn <= 0) {
            timeToSpeedUpSpawn += spawnSpeedUpPeriodSeconds;
            spawnPeriodSeconds *= spawnPeriodSpeedUpFactor;
        }
    }

    const float shipMaxSpeed = 10.f;
    const double shotCooldownSeconds = 1.0;
    const float screenWidth = 100.f;
    const float screenHeight = 50.f;
    const float bulletSpeed = 10.f;
    const float hitDistance = 5.f;

    float shipPosition = 0.f;
    float shipSpeed = 0.f;
    bool leftPressed = false;
    bool rightPressed = false;
    bool shotRequested = false;
    std::vector<Point> bullets;
    std::vector<Point> enemies;
    double secondsSinceLastShot = 0.0;
    double spawnPeriodSeconds = 3.0;
    double timeToSpawn = 3.0;
    double timeToSpeedUpSpawn = 5.f;
    double spawnSpeedUpPeriodSeconds = 5.f;
    double spawnPeriodSpeedUpFactor = 0.95;
};

int main()
{
    Game game;

    auto client = ge::Client{};
    client.config.windowTitle = "Ge geometry example";
    client.create();

    client.onKeyDownUp(
        ge::Key::Left,
        [&game] {
            game.leftPressed = true;
        }, [&game] {
            game.leftPressed = false;
        });
    client.onKeyDownUp(
        ge::Key::Right,
        [&game] {
            game.rightPressed = true;
        },
        [&game] {
            game.rightPressed = false;
        });
    client.onKeyDownUp(
        ge::Key::Space,
        [&game] {
            game.shotRequested = true;
        },
        [&game] {
            game.shotRequested = false;
        });

    const int fps = 60;
    auto timer = ge::FrameTimer{fps};

    auto circle = client.spawn<ge::Circle>();

    while (client.isAlive()) {
        client.processInput();

        if (auto framesPassed = timer(); framesPassed > 0) {
            for (auto i = framesPassed; i > 0; i--) {
                game.update(timer.delta());
                circle->x(game.shipPosition);
            }

            client.update(framesPassed * timer.delta());
            client.display();
        }

        timer.relax();
    }
}
