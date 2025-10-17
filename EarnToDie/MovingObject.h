#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

using namespace sf;
class MovingObject : public GameObject {
public:
	int damage;
	MovingObject(Shape* shapePtr, Vector2f startPosition, int dmg, Texture* tex = nullptr) : GameObject(shapePtr, startPosition, tex), damage(dmg) {}

	void update(Vector2f moveDirection) override;
	FloatRect getGlobalBounds() const; //почему конст?
};

