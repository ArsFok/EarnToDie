#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include "EnemyController.h"
#include "GameState.h"
#include "const.h"
#include "RectObject.h"

class EnemySpawner {
private:
    float& enemySpawnTimer;
    float enemySpawnRate;
    std::vector<std::unique_ptr<EnemyController>>& enemies;
    GameState& gameState;
    float speedMultiplier;

public:
    EnemySpawner(float& spawnTimer, float spawnRate,
        std::vector<std::unique_ptr<EnemyController>>& enemiesList,
        GameState& state, float multiplier = 1.0f)
        : enemySpawnTimer(spawnTimer), enemySpawnRate(spawnRate),
        enemies(enemiesList), gameState(state), speedMultiplier(multiplier) {
    }

    void update(float newSpeed) {
        if (!gameState.isPlaying()) return;

        if (enemySpawnTimer >= enemySpawnRate) {
            enemySpawnTimer = 0.0f;

            float speed = newSpeed * speedMultiplier;
            int gold = 1;
            float size = 70;
            const int spawnWidth = WINDOW_WIDTH - 485;
            const int minX = 240;

            sf::Vector2f position(static_cast<float>(minX + rand() % spawnWidth), -60.f);

            std::unique_ptr<RectObject> rectObject = std::unique_ptr<RectObject>(new RectObject(size, position, 1));
            enemies.push_back(std::unique_ptr<EnemyController>(new EnemyController(std::move(rectObject), speed)));
        }
    }

    void setSpawnRate(float rate) {
        enemySpawnRate = rate;
    }

    void setSpeedMultiplier(float multiplier) {
        speedMultiplier = multiplier;
    }

    float getSpeedMultiplier() const {
        return speedMultiplier;
    }
};