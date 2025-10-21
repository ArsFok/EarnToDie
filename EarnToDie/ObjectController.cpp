#include "ObjectController.h"
#include "const.h"

bool ObjectController::update() {
	Vector2f direction(0, 1 * speed);
	subject->update(direction);

	FloatRect bounds = subject->getGlobalBounds();
	return bounds.top > WINDOW_HEIGHT;
}
void ObjectController::draw(RenderWindow& window) {
	subject->draw(window);
}

StaticObject* ObjectController::getEnemy() {
	return subject.get();
}