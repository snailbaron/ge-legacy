#include <ge.hpp>

#include <algorithm>
#include <filesystem>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

struct Object {
    std::shared_ptr<ge::Circle> circle;
    std::shared_ptr<ge::Sprite> sprite;
    float x = 0.f;
    float y = 0.f;
};

struct Game {
    Game(ge::Scene& geometryScene, ge::Scene& spriteScene)
        : geometryScene(geometryScene)
        , spriteScene(spriteScene)
    {
        shipCircle = geometryScene.spawn<ge::Circle>();
        shipCircle->radius(shipRadius);
        shipCircle->y(0);
        shipCircle->color({.r = 255, .g = 255, .b = 255, .a = 255});
        shipCircle->pointCount(32);

        const auto shipPath = fs::path{
            "/home/snailbaron/code/ge/examples/01-starship/data/ship.png"};
        auto shipFileMap = ge::ReadOnlyFileMap{shipPath};
        shipTexture = ge::Texture{shipFileMap.content()};
        shipSprite = spriteScene.spawn<ge::Sprite>(shipTexture);
        shipSprite->frameSeconds(0.5f);
        shipSprite->addFrame(0, 0, 16, 16);
        shipSprite->addFrame(16, 0, 16, 16);
        shipSprite->pixelSize(12.f * (1.f + 2 * shipRadius) / 1920);

        const auto bulletPath = fs::path{
            "/home/snailbaron/code/ge/examples/01-starship/data/rock.png"};
        auto bulletFileMap = ge::ReadOnlyFileMap{bulletPath};
        bulletTexture = ge::Texture{bulletFileMap.content()};
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
        shipCircle->x(shipPosition);
        shipSprite->x(shipPosition);

        {
            auto it = enemies.begin();
            auto end = enemies.end();
            while (it != end) {
                if (it->y <= -0.1f) {
                    --end;
                    std::iter_swap(it, end);
                } else {
                    ++it;
                }
            }
            enemies.erase(end, enemies.end());
        }

        for (auto& enemy : enemies) {
            enemy.y -= enemySpeed * delta;
            enemy.circle->y(enemy.y);
            enemy.sprite->y(enemy.y);
            if (enemy.y <= 10) {
            }
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
            bullet->circle->y(bullet->y);
            bullet->sprite->y(bullet->y);

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
            bullet.circle = geometryScene.spawn<ge::Circle>();
            bullet.circle->color({255, 255, 255, 255});
            bullet.circle->radius(bulletRadius);
            bullet.circle->x(bullet.x);
            bullet.sprite = spriteScene.spawn<ge::Sprite>(bulletTexture);
            bullet.sprite->addFrame(0, 0, 8, 8);
            bullet.sprite->x(bullet.x);
            bullet.sprite->pixelSize(12.f * (1.f + 2 * shipRadius) / 1920);
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
            enemy.circle = geometryScene.spawn<ge::Circle>();
            enemy.circle->color({255, 100, 100, 255});
            enemy.circle->radius(enemyRadius);
            enemy.circle->x(enemy.x);
            enemy.circle->y(enemy.y);
            enemy.sprite = spriteScene.spawn<ge::Sprite>(shipTexture);
            enemy.sprite->addFrame(0, 0, 16, 16);
            enemy.sprite->addFrame(16, 0, 16, 16);
            enemy.sprite->x(enemy.x);
            enemy.sprite->y(enemy.y);
            enemy.sprite->pixelSize(12.f * (1.f + 2 * shipRadius) / 1920);
            enemy.sprite->rotation(180.f);
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
    const float bulletRadius = 0.02f;
    const float enemyRadius = 0.03f;
    const float enemySpeed = 0.2f;

    ge::Scene& geometryScene;
    ge::Scene& spriteScene;

    const double shotCooldownSeconds = 1.0;
    double secondsSinceLastShot = shotCooldownSeconds;

    int spawnPosition = 0;

    ge::Texture shipTexture;
    ge::Texture bulletTexture;

    float shipPosition = 0.f;
    float shipSpeed = 0.f;
    bool leftPressed = false;
    bool rightPressed = false;
    bool shotRequested = false;
    std::shared_ptr<ge::Circle> shipCircle;
    std::shared_ptr<ge::Sprite> shipSprite;
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

    client.sceneNumber(2);
    auto& geometryScene = client.scene(0);
    auto& spriteScene = client.scene(1);

    auto game = Game{geometryScene, spriteScene};

    auto r = game.shipRadius;
    auto w = 1.f + 2 * r;
    auto h = w * client.height() / client.width();
    geometryScene.view(-r, -r, w, h);
    spriteScene.view(-r, -r, w, h);

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
    client.onKeyDown(
        ge::Key::F1,
        [&geometryScene, &spriteScene] {
            geometryScene.visible(true);
            spriteScene.visible(false);
        });
    client.onKeyDown(
        ge::Key::F2,
        [&geometryScene, &spriteScene] {
            geometryScene.visible(false);
            spriteScene.visible(true);
        });
    client.onKeyDown(
        ge::Key::F3,
        [&geometryScene, &spriteScene] {
            geometryScene.visible(true);
            spriteScene.visible(true);
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
