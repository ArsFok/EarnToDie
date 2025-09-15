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

void createEnemies(float& enemySpawnTimer, const float enemySpawnRate, vector<unique_ptr<EnemyController>>& enemies) {
    if (enemySpawnTimer >= enemySpawnRate) {
        enemySpawnTimer = 0.0f;

        bool isCircle = rand() % 2 == 0;
        float speed = 0.05f;
        int damage = 1;

        Vector2f position(rand() % (WINDOW_WIDTH - 60), -60);

        unique_ptr<MovingObject> enemy;

        if (isCircle) {
            float radius = 15 + rand() % 20;
            enemy = make_unique<CircleObject>(radius, position, damage);
        }
        else {
            float size = 20 + rand() % 30;
            enemy = make_unique<RectObject>(size, position, damage);
        }
        enemies.push_back(make_unique<EnemyController>(move(enemy), speed));
    }
}
int main()
{
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "EarnToDie");

    GameState gameState;

    EntityController controller;
    
    vector<unique_ptr<EnemyController>> enemies;
    const float enemySpawnRate = 0.5f;
    float enemySpawnTimer = 0.0f;
    Clock enemySpawnClock;

    while (window.isOpen()) {
        window.clear();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        float deltaTime = enemySpawnClock.restart().asSeconds();
        enemySpawnTimer += deltaTime;
        createEnemies(enemySpawnTimer, enemySpawnRate, enemies);
        for (auto it = enemies.begin(); it != enemies.end();) {
            bool shouldRemove = (*it)->update();
            if (shouldRemove) {
                gameState.decreaseFuel(1);
                it = enemies.erase(it);
            }
            else {
                ++it;
            }
        }
        for (auto it = enemies.begin(); it != enemies.end();) {
            if ((*it)->getEnemy()->checkCollision(controller.getEntity()->shape)) {
                gameState.decreaseHealth(1);
                it = enemies.erase(it);
            }
            else {
                ++it;
            }
        }
        for (const auto& enemy : enemies) {
            enemy->draw(window);
        }

        gameState.draw(window);
        controller.update(window);

        window.display();
    }
    return 0;
}