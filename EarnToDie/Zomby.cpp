#include "Zomby.h"


Zomby::Zomby() : rng(std::random_device{}()), distributionX(100, 700), velocity(200.f) {

	shape = RectangleShape(Vector2f(50.f, 50.f));
	shape.setOrigin(25.f, 25.f);
	int randomX = distributionX(rng);
	shape.setPosition(randomX, 20);
	shape.setFillColor(sf::Color::Red);

	position = shape.getPosition();
}
