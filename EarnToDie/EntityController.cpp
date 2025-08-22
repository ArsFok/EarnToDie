#include "EntityController.h"
#include "Entity.h"

EntityController::EntityController() {
	entity = new Entity();
    zomby = new Zomby();
}

void EntityController::update(RenderWindow& window) {

	checkInputs();
    zombyMove();
	window.draw(entity->shape);
    window.draw(zomby->shape);
}
void EntityController::zombyMove() {
    dt = clock.restart().asSeconds();

    zomby->position.y = zomby->velocity * dt;

    if (zomby->position.y > 600 + zomby->shape.getGlobalBounds().height / 2) {
        zomby->position.y = -zomby->shape.getGlobalBounds().height;
        zomby->position.x = static_cast<float>(zomby->distributionX(zomby->rng));
    }
    zomby->shape.setPosition(zomby->position);

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