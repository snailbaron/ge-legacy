#include <ge.hpp>

#include <algorithm>
#include <vector>

#include <iostream>

struct Point {
    float x;
    float y;
};

struct Game {
    void update(float delta)
    {
        float shipTargetSpeed = (rightPressed - leftPressed) * shipMaxSpeed;
        if (shipTargetSpeed > shipSpeed) {
            shipSpeed =
                std::min(shipTargetSpeed, shipSpeed + shipAcceleration * delta);
        } else if (shipTargetSpeed < shipSpeed) {
            shipSpeed =
                std::max(shipTargetSpeed, shipSpeed - shipAcceleration * delta);
        }

        shipPosition += shipSpeed * delta;
        if (shipPosition < 0.f) {
            shipPosition = 0.f;
            shipSpeed = 0.f;
        } else if (shipPosition > 1.f) {
            shipPosition = 1.f;
            shipSpeed = 0.f;
        }
        std::cerr << "target speed: " << shipTargetSpeed <<
            "; ship speed: " << shipSpeed <<
            "; position: " << shipPosition << "\n";

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

    const float shipAcceleration = 1000.f;
    const float shipMaxSpeed = 1.f;
    const double shotCooldownSeconds = 1.0;
    const float bulletSpeed = 1.f;
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

    client.scene.view(0, 0, 1, client.heightToWidthRatio());

    auto circle = client.scene.spawn<ge::Circle>();
    circle->radius(0.05f);
    circle->color({.r = 255, .g = 255, .b = 255, .a = 255});
    circle->pointCount(32);

    while (client.isAlive()) {
        client.processInput();

        if (auto framesPassed = timer(); framesPassed > 0) {
            for (auto i = framesPassed; i > 0; i--) {
                game.update(timer.delta());
                circle->x(game.shipPosition);
            }

            client.update(framesPassed * timer.delta());
        }

        client.display();

        timer.relax();
    }
}
