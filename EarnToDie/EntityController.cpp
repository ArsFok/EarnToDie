#include "EntityController.h"
#include <iostream>
#include "Entity.h"


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
    float cooldownTime = inputCooldown.getElapsedTime().asSeconds();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && cooldownTime > 0.2f) {
        m_shouldRestart = true;
        m_isFinal = false;
        inputCooldown.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && cooldownTime > 0.2f) {
        m_isFinal = !m_isFinal;
        inputCooldown.restart();
    }
    if (m_isFinal) {
        return;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && cooldownTime > 0.2f) {
        m_isPaused = !m_isPaused;
        inputCooldown.restart();
    }
    bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
    if (shiftPressed != m_isShiftPressed) {
        m_isShiftPressed = shiftPressed;
        updateGameSpeed();
    }
    if (m_isPaused) {
        return;
    }
    Vector2f direction;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        direction.x = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x = 1;
    }
    else {
        direction.x = -0;
    }
    /*
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        direction.y = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        direction.y = 1;
    }
    else {
        direction.y = 0;
    }
    */
    if (direction.x == 0 && direction.y == 0) {
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