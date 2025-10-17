#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class GameObject{
public:
	Shape* shape;
	Texture* texture;

	GameObject(Shape* shapePtr, Vector2f startPosition, Texture* tex = nullptr) : shape(shapePtr), texture(tex) {
		shape->setPosition(startPosition); //не доконца понял, как работает этот конструктор

		if (texture) {
			shape->setTexture(texture);
		}
	}

	virtual ~GameObject() {
		delete shape;
		delete texture;
	}
	// почему виртуальные функции
	virtual void update(Vector2f moveDirection) = 0;
	virtual void draw(RenderWindow& windows) const; //почему конст?

	bool checkCollision(const RectangleShape& player) const;
	Vector2f getPosition();
};

