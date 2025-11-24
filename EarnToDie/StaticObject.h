#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

using namespace sf;
class StaticObject : public GameObject {
public:
	StaticObject(Shape* shapePtr, Vector2f startPosition) : GameObject(shapePtr, startPosition) {}

	void update(Vector2f moveDirection) final override;
	FloatRect getGlobalBounds() const final override;
};

