#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class GameObject{
public:
	Shape* shape;

	GameObject(Shape* shapePtr, Vector2f startPosition) : shape(shapePtr) {
		shape->setPosition(startPosition); //не доконца понял, как работает этот конструктор
	}

	virtual ~GameObject() {
		delete shape;
	}
	// почему виртуальные функции
	virtual void update(Vector2f moveDirection) = 0;
	virtual void draw(RenderWindow& windows) const; //почему конст?
	virtual virtual FloatRect getGlobalBounds() const = 0;

	bool checkCollision(const RectangleShape& player) const;
	Vector2f getPosition();
};

