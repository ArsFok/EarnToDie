#pragma once
#include <SFML/Graphics.hpp>
#include "MovingObject.h"

using namespace sf;

class CircleObject :public MovingObject {
public:
    CircleObject(float radius, Vector2f startPosition, int damage)
        : MovingObject(new CircleShape(radius), startPosition, damage) {
        shape->setOrigin(radius / 2, radius / 2);
        shape->setFillColor(sf::Color::Green);
    }
};

