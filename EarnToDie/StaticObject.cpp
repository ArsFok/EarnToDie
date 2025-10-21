#include "StaticObject.h"
#include <SFML/Graphics.hpp>

void StaticObject::update(Vector2f moveDirection) {
	shape->move(moveDirection);
}
FloatRect StaticObject::getGlobalBounds() const {
	return shape->getGlobalBounds();
}