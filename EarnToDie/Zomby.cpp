#include "Zomby.h"


Zomby::Zomby() : rng(std::random_device{}()), distributionX(0, 800), velocity(0.1f) {

	shape = RectangleShape(Vector2f(50.f, 50.f));
	shape.setOrigin(25.f, 25.f);
	int randomX = distributionX(rng);
	shape.setPosition(randomX, 20);
	shape.setFillColor(sf::Color::Red);

	position = shape.getPosition();
}

void Zomby::draw(RenderWindow& window) {
	window.draw(shape);
}

void Zomby::update(float dt, const RectangleShape& player) {
	if (!player.getGlobalBounds().intersects(shape.getGlobalBounds())) {
		float newY = position.y + velocity * dt;
		if (newY + shape.getGlobalBounds().height / 2 < 600) {
			position.y = newY;
			shape.setPosition(position);
		}
	}
}