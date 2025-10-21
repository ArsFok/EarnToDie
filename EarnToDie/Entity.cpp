#include "Entity.h"
#include "const.h"
#include <iostream>

Entity::Entity()
{
    size = 100.f;
    speed = sf::Vector2f(1.0f, 1.0f);

    RectangleShape rect(Vector2f(BOX_HEIGHT, BOX_WIDTH));
    rect.setOrigin(50.f, 50.f);
    rect.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - 100);
    rect.setFillColor(sf::Color::Yellow);
    shape = rect;
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