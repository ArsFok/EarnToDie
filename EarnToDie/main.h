#pragma once

void createEnemies(float& enemySpawnTimer, const float enemySpawnRate, std::vector<std::unique_ptr<EnemyController>>& enemies);