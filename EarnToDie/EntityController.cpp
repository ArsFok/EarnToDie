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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && inputCooldown.getElapsedTime() > cooldownTime && !m_isFinal) {
        m_isPaused = !m_isPaused;
        inputCooldown.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && inputCooldown.getElapsedTime() > cooldownTime ) {
        m_shouldRestart = true;
        m_isFinal = false;
        inputCooldown.restart();
    }
    if ((m_isPaused || m_isFinal) && inputCooldown.getElapsedTime() > milliseconds(150)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            movePauseMenuUp();
            inputCooldown.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            movePauseMenuDown();
            inputCooldown.restart();
        }
        return;
    }
    bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
    if (shiftPressed != m_isShiftPressed) {
        m_isShiftPressed = shiftPressed;
        updateGameSpeed();
    }
    if (m_isPaused || m_isFinal) {
        return;
    }
    Vector2f direction;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction.x = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
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