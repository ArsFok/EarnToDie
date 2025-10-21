#pragma once
#include <SFML/Graphics.hpp>
#include "StaticObject.h"

using namespace sf;

class CircleObject :public StaticObject {
public:
    CircleObject(float radius, Vector2f startPosition)
        : StaticObject(new CircleShape(radius), startPosition) {
        shape->setOrigin(radius / 2, radius / 2);
        shape->setFillColor(sf::Color::Green);
    }
};

