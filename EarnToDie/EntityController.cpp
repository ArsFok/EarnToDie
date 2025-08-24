#include "EntityController.h"
#include "Entity.h"
#include "Zomby.h"
#include <stdbool.h>
#include <iostream>
#include <cmath>

EntityController::EntityController() {
	entity = new Entity();
    zomby = new Zomby();

    for (int i = 0; i < 1; i++) {
        zombies.push_back(new Zomby());
        zombies.back()->position.y = static_cast<float>(-100 * (i + 1));
    }
}

void EntityController::update(RenderWindow& window) {
    dt = clock.restart().asSeconds();

    if (spawnClock.getElapsedTime().asSeconds() > 3.0f) {
        zombies.push_back(new Zomby());
        spawnClock.restart();
    }

	checkInputs();
    zombyMove();

    for (auto& zombie : zombies) {
        zomby = zombie;
        if (checkCollision()) {
            handleCollision();
            break;
        }
    }
	window.draw(entity->shape);
    for (auto& zombie : zombies) {
        window.draw(zombie->shape);
    }
}
void EntityController::zombyMove() {

    for (auto it = zombies.begin(); it != zombies.end(); ) {
        Zomby* zombie = *it;
        zombie->position.y += zombie->velocity * dt;

        if (zombie->position.y > 600 + zombie->shape.getGlobalBounds().height) {

            delete zombie;
            it = zombies.erase(it);
            zombies.push_back(new Zomby());
        }
        else {
            zombie->shape.setPosition(zombie->position);
            ++it;
        }
    }
}
bool EntityController::checkCollision() {
    return boundingBoxCollision();
}
bool EntityController::boundingBoxCollision() {
    // AABB (Axis-Aligned Bounding Box) проверка
    sf::FloatRect entityBounds = entity->shape.getGlobalBounds();
    sf::FloatRect zombieBounds = zomby->shape.getGlobalBounds();

    return entityBounds.intersects(zombieBounds);
}
void EntityController::handleCollision() {
    zomby->position.y = -zomby->shape.getGlobalBounds().height;
    zomby->position.x = static_cast<float>(zomby->distributionX(zomby->rng));
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

    for (auto& zombie : zombies) {
        delete zombie;
    }
    zombies.clear();
}