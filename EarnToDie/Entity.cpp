#include "Entity.h"
#include <iostream>

Entity::Entity() {
    size = 100.f;
    speed = sf::Vector2f(0.1f, 0.1f);

    RectangleShape rect(Vector2f(100, 100));
    rect.setOrigin(100.f / 2, 100.f / 2);
    rect.setFillColor(sf::Color::Green);
    shape = rect;
}

void Entity::move(Vector2f direction)
{
    shape.setPosition(shape.getPosition() + direction);

    float halh_size = size / 2;

    float newX = shape.getPosition().x;
    float newY = shape.getPosition().y;

    if (newX - halh_size < 0) {
        newX = halh_size;
    }
    else if (newX + halh_size > 800) {
        newX = 800 - halh_size;
    }

    if (newY - halh_size < 0) {
        newY = halh_size;
    }
    else if (newY + halh_size > 600) {
        newY = 600 - halh_size;
    }
    shape.setPosition(newX, newY);
}