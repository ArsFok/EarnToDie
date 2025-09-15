#include "GameObject.h"
#include <SFML/Graphics.hpp>

void GameObject::draw(RenderWindow& window) const {
	window.draw(*shape);
}

bool GameObject::checkCollision(const RectangleShape& player) const {
	return shape->getGlobalBounds().intersects(player.getGlobalBounds());
}

Vector2f GameObject::getPosition() {
	return shape->getPosition();
}