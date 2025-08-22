#include "Entity.h"
#include <iostream>

Entity::Entity() {
    size = 100.f;
    speed = sf::Vector2f(0.1f, 0.1f);

    RectangleShape rect(Vector2f(100, 100));
    rect.setOrigin(50.f, 50.f);
    rect.setPosition(400, 400);
    rect.setFillColor(sf::Color::Green);

    RectangleShape zomby(Vector2f(50, 50));
    zomby.setOrigin(50.f, 50.f);
    zomby.setPosition(100, 100);
    zomby.setFillColor(sf::Color::Red);
    shape = rect;
    Zomby = zomby;
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