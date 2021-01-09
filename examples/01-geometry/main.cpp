#include <ge.hpp>

#include <algorithm>
#include <utility>
#include <vector>

struct Object {
    std::shared_ptr<ge::Circle> circle;
    float x = 0.f;
    float y = 0.f;
};

struct Game {
    Game(ge::Client& client)
        : client(client)
    {
        ship = client.scene.spawn<ge::Circle>();
        ship->radius(shipRadius);
        ship->y(0);
        ship->color({.r = 255, .g = 255, .b = 255, .a = 255});
        ship->pointCount(32);
    }

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
        ship->x(shipPosition);

        for (auto& enemy : enemies) {
            enemy.y -= enemySpeed * delta;
            enemy.circle->y(enemy.y);
            if (enemy.y <= 0) {
                lost = true;
            }
        }

        for (auto bullet = bullets.begin(); bullet != bullets.end(); ) {
            bullet->y += bulletSpeed * delta;
            bool hit = false;
            for (auto enemy = enemies.begin(); enemy != enemies.end(); ) {
                auto dx = bullet->x - enemy->x;
                auto dy = bullet->y - enemy->y;
                const auto hitDistance = enemyRadius + bulletRadius;
                if (dx * dx + dy * dy <= hitDistance * hitDistance) {
                    hit = true;
                    std::iter_swap(enemy, std::prev(enemies.end()));
                    enemies.resize(enemies.size() - 1);
                } else {
                    ++enemy;
                }
            }
            bullet->circle->x(bullet->x);
            bullet->circle->y(bullet->y);

            if (hit) {
                std::iter_swap(bullet, std::prev(bullets.end()));
                bullets.resize(bullets.size() - 1);
            } else {
                ++bullet;
            }
        }

        secondsSinceLastShot += delta;
        if (shotRequested && secondsSinceLastShot >= shotCooldownSeconds) {
            secondsSinceLastShot = 0.0;

            auto bullet = Object{};
            bullet.x = shipPosition;
            bullet.circle = client.scene.spawn<ge::Circle>();
            bullet.circle->color({255, 255, 255, 255});
            bullet.circle->radius(bulletRadius);
            bullet.circle->x(bullet.x);
            bullets.push_back(std::move(bullet));
        }

        timeToSpawn -= delta;
        while (timeToSpawn <= 0) {
            timeToSpawn += spawnPeriodSeconds;

            spawnPosition = (spawnPosition + 7) % 11;
            float spawnX = 1.f * spawnPosition / 10.f;
            auto enemy = Object{};
            enemy.x = spawnX;
            enemy.y = 0.5f;
            enemy.circle = client.scene.spawn<ge::Circle>();
            enemy.circle->color({255, 100, 100, 255});
            enemy.circle->radius(enemyRadius);
            enemy.circle->x(enemy.x);
            enemy.circle->y(enemy.y);
            enemies.push_back(std::move(enemy));
        }

        timeToSpeedUpSpawn -= delta;
        while (timeToSpeedUpSpawn <= 0) {
            timeToSpeedUpSpawn += spawnSpeedUpPeriodSeconds;
            spawnPeriodSeconds *= spawnPeriodSpeedUpFactor;
        }
    }

    const float shipAcceleration = 3.f;
    const float shipMaxSpeed = 0.7f;
    const float bulletSpeed = 0.7f;
    const float shipRadius = 0.05f;
    const float bulletRadius = 0.01f;
    const float enemyRadius = 0.03f;
    const float enemySpeed = 0.2f;

    ge::Client& client;

    const double shotCooldownSeconds = 1.0;
    double secondsSinceLastShot = shotCooldownSeconds;

    int spawnPosition = 0;

    float shipPosition = 0.f;
    float shipSpeed = 0.f;
    bool leftPressed = false;
    bool rightPressed = false;
    bool shotRequested = false;
    std::shared_ptr<ge::Circle> ship;
    std::vector<Object> bullets;
    std::vector<Object> enemies;
    double spawnPeriodSeconds = 3.0;
    double timeToSpawn = 0.0;
    double timeToSpeedUpSpawn = 5.f;
    double spawnSpeedUpPeriodSeconds = 5.f;
    double spawnPeriodSpeedUpFactor = 0.95;
    bool lost = false;
};

int main()
{
    auto client = ge::Client{};
    client.config.windowTitle = "Ge geometry example";
    client.create();

    auto game = Game{client};

    auto r = game.shipRadius;
    auto w = 1.f + 2 * r;
    auto h = w * client.height() / client.width();
    client.scene.view(-r, -r, w, h);

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

    while (client.isAlive()) {
        client.processInput();

        if (auto framesPassed = timer(); framesPassed > 0) {
            for (auto i = framesPassed; i > 0; i--) {
                game.update(timer.delta());
            }
            if (game.lost) {
                client.kill();
            }

            client.update(framesPassed * timer.delta());
        }

        client.display();

        timer.relax();
    }
}
