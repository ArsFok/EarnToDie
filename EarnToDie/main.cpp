#include <SFML/Graphics.hpp>
#include "EntityController.h"
#include "const.h"
#include "EnemyController.h"
#include "CircleObject.h"
#include "RectObject.h"
#include "main.h"
#include "GameState.h"
#include <vector>
#include <ctime>
#include <memory>
#include <cstdlib>
    
using namespace sf;
using namespace std;

void createEnemies(float& enemySpawnTimer, const float enemySpawnRate, vector<unique_ptr<EnemyController>>& enemies, GameState& gameState) {
    if (!gameState.isPlaying()) return;
    if (enemySpawnTimer >= enemySpawnRate) {
        enemySpawnTimer = 0.0f;

        bool isCircle = rand() % 2 == 0;
        float speed = 1.0f;
        int gold = 1;

        Vector2f position(240 + rand() % (WINDOW_WIDTH - 225 - 240), -60);

        unique_ptr<MovingObject> enemy;

        if (isCircle) {
            float radius = 15 + rand() % 20;
            enemy = make_unique<CircleObject>(radius, position, gold);
        }
        else {
            float size = 20 + rand() % 30;
            enemy = make_unique<RectObject>(size, position, gold + 5);
        }
        enemies.push_back(make_unique<EnemyController>(move(enemy), speed));
    }
}

void resetGame(GameState& gameState, vector<unique_ptr<EnemyController>>& enemies,
    EntityController& controller, float& enemySpawnTimer) {
    gameState.restartGame();
    enemies.clear();
    enemySpawnTimer = 0.0f;
}
int main()
{
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "EarnToDie");

    Texture backgroundTexture;

    if (!backgroundTexture.loadFromFile("background.jpg")) {
        cout << "Failed to load background image!" << endl;
    }
    Sprite background1(backgroundTexture);
    Sprite background2(backgroundTexture);

    Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(WINDOW_WIDTH) / textureSize.x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / textureSize.y;

    background1.setScale(scaleX, scaleY);
    background2.setScale(scaleX, scaleY);

    background1.setPosition(0, 0);
    background2.setPosition(0, -static_cast<float>(WINDOW_HEIGHT));

    float backgroundSpeed = 400.0f;
    float backgroundY1 = 0.0f;
    float backgroundY2 = -static_cast<float>(WINDOW_HEIGHT);

    GameState gameState;

    EntityController controller;
    
    vector<unique_ptr<EnemyController>> enemies;
    const float enemySpawnRate = 0.5f;
    float enemySpawnTimer = 0.0f;
    int dist = 0;
    int fuel = 350; // бак размером 1 ур: 350 л, 2 ур: 450, 3 ур: 500, 4 ур: 600
    Clock enemySpawnClock;

    while (window.isOpen()) {
        window.clear();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        if (controller.isGamePaused() != gameState.isPaused()) {
            gameState.setPaused(controller.isGamePaused());
        }
        if (controller.shouldRestartGame()) {
            resetGame(gameState, enemies, controller, enemySpawnTimer);
            controller.resetRestartFlag();
        }
        if (controller.isGameFinal() && !gameState.isGameOver()) {
            gameState.setGameOver();
        }
        window.clear();

        if (gameState.isPlaying() && !controller.isGamePaused() && !controller.isGameFinal()) {
            float deltaTime = enemySpawnClock.restart().asSeconds();
            float scaledDeltaTime = deltaTime * controller.getGameSpeed();
            int speed = controller.getGameSpeed() * 20;

            backgroundY1 += backgroundSpeed * scaledDeltaTime;
            backgroundY2 += backgroundSpeed * scaledDeltaTime;

            if (backgroundY1 >= WINDOW_HEIGHT) {
                backgroundY1 = backgroundY2 - WINDOW_HEIGHT;
            }
            if (backgroundY2 >= WINDOW_HEIGHT) {
                backgroundY2 = backgroundY1 - WINDOW_HEIGHT;
            }

            background1.setPosition(0, backgroundY1);
            background2.setPosition(0, backgroundY2);

            enemySpawnTimer += scaledDeltaTime;
            createEnemies(enemySpawnTimer, enemySpawnRate, enemies, gameState);
            for (auto it = enemies.begin(); it != enemies.end();) {
                bool shouldRemove = (*it)->update();
                dist = speed * enemySpawnTimer;
                if (shouldRemove) {
                    gameState.decreaseFuel(fuel);
                    gameState.decreaseSpeed(speed);
                    gameState.decreaseDist(dist);
                    it = enemies.erase(it);
                }
                else {
                    ++it;
                }
            }
            for (auto it = enemies.begin(); it != enemies.end();) {
                if ((*it)->getEnemy()->checkCollision(controller.getEntity()->shape)) {
                    gameState.decreaseGold(10);
                    it = enemies.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
        window.draw(background1);
        window.draw(background2);

        for (const auto& enemy : enemies) {
            enemy->draw(window);
        }

        gameState.draw(window);
        controller.update(window);

        window.display();
    }
    return 0;
}