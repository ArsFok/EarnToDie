#include "Entity.h"
#include <iostream>

Entity::Entity()
{
    size = 100.f;
    speed = sf::Vector2f(1.0f, 1.0f);

    if (!carTexture.loadFromFile("car.png")) {
        std::cout << "Failed to load background image!" << std::endl;
        shape.setFillColor(sf::Color::Green);
    }
    else {
        shape.setTexture(&carTexture);
    }

    shape.setSize(Vector2f(BOX_HEIGHT, BOX_WIDTH));
    shape.setOrigin(50.f, 50.f);
    shape.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - 100);
}


void Entity::move(Vector2f direction)
{
    shape.setPosition(shape.getPosition() + Vector2f(direction.x * speed.x, direction.y * speed.y));

    float halh_size = size / 2;

    float newX = shape.getPosition().x;
    float newY = shape.getPosition().y;

    if (newX - halh_size < 240) {
        newX = halh_size + 240;
    }
    else if (newX + halh_size > WINDOW_WIDTH - 225) {
        newX = WINDOW_WIDTH - halh_size - 225;
    }
    shape.setPosition(newX, newY);
}