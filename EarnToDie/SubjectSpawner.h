#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include "ObjectController.h"
#include "CircleObject.h"
#include "GameState.h"
#include "const.h"

class SubjectSpawner {
private:
    float& subjectSpawnTimer;
    float subjectSpawnRate;
    std::vector<std::unique_ptr<ObjectController>>& subjects;
    GameState& gameState;
    float speedMultiplier;

public:
    SubjectSpawner(float& spawnTimer, float spawnRate,
        std::vector<std::unique_ptr<ObjectController>>& subjectsList,
        GameState& state, float multiplier = 1.0f)
        : subjectSpawnTimer(spawnTimer), subjectSpawnRate(spawnRate),
        subjects(subjectsList), gameState(state), speedMultiplier(multiplier) {
    }

    void update(float newSpeed) {
        if (!gameState.isPlaying()) return;

        if (subjectSpawnTimer >= subjectSpawnRate) {
            subjectSpawnTimer = 0.0f;

            float speed = newSpeed * speedMultiplier;

            sf::Vector2f position(static_cast<float>(240 + rand() % (WINDOW_WIDTH - 495)), -60.f);
            float radius = 30;

            // Создаем CircleObject
            std::unique_ptr<CircleObject> circleObject = std::unique_ptr<CircleObject>(new CircleObject(radius, position));
            subjects.push_back(std::unique_ptr<ObjectController>(new ObjectController(std::move(circleObject), speed)));
        }
    }

    void setSpeedMultiplier(float multiplier) {
        speedMultiplier = multiplier;
    }

    float getSpeedMultiplier() const {
        return speedMultiplier;
    }
};