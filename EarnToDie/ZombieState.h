#pragma once
#include <SFML/System.hpp>

class ZombieState {
public:
    enum class State {
        ALIVE,
        KNOCKBACK,
        DEAD
    };

    ZombieState();

    void triggerKnockback(const sf::Vector2f& direction, float force, float duration);
    void triggerDeath();
    void update(float deltaTime);

    State getState() const { return currentState; }
    bool isAlive() const { return currentState == State::ALIVE; }
    bool isKnockback() const { return currentState == State::KNOCKBACK; }
    bool isDead() const { return currentState == State::DEAD; }

    sf::Vector2f getKnockbackVelocity() const { return knockbackVelocity; }
    float getKnockbackTimer() const { return knockbackTimer; }

private:
    State currentState;
    sf::Vector2f knockbackVelocity;
    float knockbackDuration;
    float knockbackTimer;
};