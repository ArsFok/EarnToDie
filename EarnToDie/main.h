#pragma once

#include <vector>
#include <memory>
#include "EnemyController.h"
#include "ObjectController.h"
#include "GameState.h"
#include "EntityController.h"

using namespace std;

void createEnemies(float& enemySpawnTimer, const float enemySpawnRate, vector<unique_ptr<EnemyController>>& enemies, GameState& gameState);
void createSubject(float& enemySpawnTimer, const float enemySpawnRate, vector<unique_ptr<ObjectController>>& subjects, GameState& gameState);
void resetGame(GameState& gameState, vector<unique_ptr<EnemyController>>& enemies, vector<unique_ptr<ObjectController>>& subjects, EntityController& controller,
    float& enemySpawnTimer, float& subjectSpawnTimer, int& distance);