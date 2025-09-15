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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        direction.y = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        direction.y = 1;
    }
    else {
        direction.y = 0;
    }

    if (direction.x == 0 && direction.y == 0) {
        return;
    }

    entity->move(direction);
}

EntityController::~EntityController() {
    delete entity;
}
