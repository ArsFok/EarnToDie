#include "EntityController.h"
#include "Entity.h"

EntityController::EntityController() {
	entity = new Entity();
}

void EntityController::update(RenderWindow& window) {
	checkInputs();
	window.draw(entity->shape);
}
void EntityController::checkInputs() {
	inputMove();
}
void  EntityController::inputMove() {
	Vector2f direction;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction.x = -0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction.x = 0.1f;
    }
    else {
        direction.x = 0.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        direction.y = -0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        direction.y = 0.1f;
    }
    else {
        direction.y = 0.f;
    }

    if (direction.x == 0 && direction.y == 0) {
        return;
    }

    entity->move(direction);
}
EntityController::~EntityController() {
	delete entity;
}