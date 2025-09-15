#include "EnemyController.h"
#include "const.h"

bool EnemyController::update() {
	Vector2f direction(0, 1 * speed);
	enemy->update(direction);

	FloatRect bounds = enemy->getGlobalBounds();
	return bounds.top > WINDOW_HEIGHT;
}
void EnemyController::draw(RenderWindow& window) {
	enemy->draw(window);
}

MovingObject* EnemyController::getEnemy() {
	return enemy.get();
}