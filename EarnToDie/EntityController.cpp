#include "EntityController.h"
#include "Entity.h"

EntityController::EntityController() {
	entity = new Entity();
    zomby = new Zomby();
}

void EntityController::update(RenderWindow& window) {
    dt = clock.restart().asSeconds();

	checkInputs();
    zomby->update(dt, entity->shape);

	window.draw(entity->shape);
    zomby->draw(window);
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
    delete zomby;
}