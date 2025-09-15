#pragma once
#include <SFML/Graphics.hpp>
#include "MovingObject.h"

using namespace sf;

class RectObject : public MovingObject {
public:
    RectObject(float size, Vector2f startPosition, int damage)
        : MovingObject(new RectangleShape(Vector2f(size, size)), startPosition, damage) {
        shape->setOrigin(size / 2, size / 2);
        shape->setFillColor(sf::Color::Red);
    }
};