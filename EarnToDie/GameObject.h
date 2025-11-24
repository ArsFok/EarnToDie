#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class GameObject{
public:
	Shape* shape;

	GameObject(Shape* shapePtr, Vector2f startPosition) : shape(shapePtr) {
		shape->setPosition(startPosition);
	}

	virtual ~GameObject() {
		delete shape;
	}
	virtual void update(Vector2f moveDirection) = 0;
	virtual void draw(RenderWindow& windows) const;
	virtual FloatRect getGlobalBounds() const = 0;

	bool checkCollision(const RectangleShape& player) const;
	Vector2f getPosition();
};

