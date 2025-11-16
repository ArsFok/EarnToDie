#include "ZombieState.h"
#include <iostream>

ZombieState::ZombieState()
    : currentState(State::ALIVE)
    , knockbackVelocity(0, 0)
    , knockbackDuration(0)
    , knockbackTimer(0) {
}

void ZombieState::triggerKnockback(const sf::Vector2f& direction, float force, float duration) {
    if (currentState != State::DEAD) {
        currentState = State::KNOCKBACK;
        knockbackVelocity = direction * force;
        knockbackDuration = duration;
        knockbackTimer = 0;
    }
}

void ZombieState::triggerDeath() {
    currentState = State::DEAD;
}

void ZombieState::update(float deltaTime) {
    if (currentState == State::KNOCKBACK) {
        float oldTimer = knockbackTimer;
        knockbackTimer += deltaTime;

        // Плавное затухание откидывания
        float progress = knockbackTimer / knockbackDuration;
        float fadeFactor = 1.0f - (progress * progress); // Квадратичное затухание

        knockbackVelocity = knockbackVelocity * fadeFactor;

        if (knockbackTimer >= knockbackDuration) {
            currentState = State::ALIVE;
            std::cout << "DEBUG: Knockback finished, enemy alive" << std::endl;
        }
    }
}