#include "MovingObject.h"
#include <SFML/Graphics.hpp>

void MovingObject::update(Vector2f moveDirection) {
	shape->move(moveDirection);
}

FloatRect MovingObject::getGlobalBounds() const {
	return shape->getGlobalBounds();
}