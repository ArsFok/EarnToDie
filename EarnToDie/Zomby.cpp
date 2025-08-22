#include "Zomby.h"


Zomby::Zomby() : rng(std::random_device{}()), distributionX(100, 700), velocity(200.f) {

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

void Zomby::update(float dt, const RectangleShape& plaer) {
	position.y += velocity * dt;

	if (position.y > 600 + shape.getGlobalBounds().height / 2) {
		position.y = -shape.getGlobalBounds().height;
		position.x = static_cast<float>(distributionX(rng));
	}
	shape.setPosition(position);
}