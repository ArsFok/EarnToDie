#include <iostream>
#include "EnemyController.h"
#include "const.h"

EnemyController::EnemyController(std::unique_ptr<MovingObject> enemyObj, float spd)
    : enemy(std::move(enemyObj))
    , speed(spd) {
}

bool EnemyController::update() {
    // Обновляем состояние зомби
    zombieState.update(1.0f / 60.0f); // 60 FPS

    // Движение зависит от состояния
    sf::Vector2f direction(0, 1 * speed);   

    if (zombieState.isKnockback()) {
        // Добавляем откидывание к обычному движению
        direction += zombieState.getKnockbackVelocity();
    }

    enemy->update(direction);

    sf::FloatRect bounds = enemy->getGlobalBounds();

    if (zombieState.isDead() && bounds.top > WINDOW_HEIGHT) {
        markedForRemoval = true;
    }
    return markedForRemoval;
}
void EnemyController::draw(sf::RenderWindow& window) {
    if (zombieState.isDead()) {
        // Эффект смерти
        enemy->setRotation(90.0f);
        enemy->setColor(sf::Color(128, 128, 128, 180));
    }
    else if (zombieState.isKnockback()) {
        // Эффект отскока
        float rotation = std::sin(zombieState.getKnockbackTimer() * 15.0f) * 30.0f;
        enemy->setRotation(rotation);
        enemy->setColor(sf::Color(255, 100, 100));
    }
    else {
        // Нормальное состояние
        enemy->setRotation(0.0f);
        enemy->setColor(sf::Color::White);
    }

    enemy->draw(window);
}


MovingObject* EnemyController::getEnemy() {
    return enemy.get();
}

void EnemyController::applyKnockback(const sf::Vector2f& direction, float force, float duration) {
    zombieState.triggerKnockback(direction, force, 3.0f);
}

void EnemyController::kill() {
    zombieState.triggerDeath();
}

bool EnemyController::shouldRemove() const {
    return markedForRemoval;
}

void EnemyController::takeDamage() {
    if (!zombieState.isDead()) {
        hitCount++;
        std::cout << "=== DAMAGE TAKEN ===" << std::endl;
        std::cout << "Enemy hit: " << hitCount << "/" << maxHits << std::endl;
        std::cout << "Address: " << this << std::endl;
        std::cout << "IsDead: " << zombieState.isDead() << std::endl;
        std::cout << "IsKnockback: " << zombieState.isKnockback() << std::endl;

        if (hitCount >= maxHits) {
            std::cout << "=== ENEMY KILLED! Hit limit reached ===" << std::endl;
            zombieState.triggerDeath();
        }
        else {
            std::cout << "=== Enemy still alive ===" << std::endl;
        }
    }
    else {
        std::cout << "=== Enemy already dead, no damage taken ===" << std::endl;
    }
}