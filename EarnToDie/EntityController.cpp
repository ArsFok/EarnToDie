#include "EntityController.h"
#include <iostream>
#include "Entity.h"
#include "GameState.h"


EntityController::EntityController()
{
    entity = new Entity();
}

void EntityController::update(RenderWindow& window)
{
    checkInputs();
    window.draw(entity->shape);
}

Entity* EntityController::getEntity()
{
    return entity;
}

void EntityController::checkInputs()
{
    inputMove();
}

void EntityController::inputMove()
{
    static Clock inputCooldown;
    const Time cooldownTime = milliseconds(150);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && inputCooldown.getElapsedTime() > cooldownTime ) {
        m_shouldRestart = true;
        m_isFinal = false;
        inputCooldown.restart();
    }
    bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
    if (shiftPressed != m_isShiftPressed) {
        m_isShiftPressed = shiftPressed;
        updateGameSpeed();
    }
    Vector2f direction;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)|| sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x = 1;
    }
    else {
        direction.x = -0;
    }
    if (direction.x == 0) {
        return;
    }
    entity->move(direction);
}

void EntityController::updateGameSpeed() {
    if (m_isShiftPressed) {
        m_gameSpeed = m_baseGameSpeed * 2.0f;
    }
    else {
        m_gameSpeed = m_baseGameSpeed;
    }
}

EntityController::~EntityController() {
    delete entity;
}