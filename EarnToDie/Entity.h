#pragma once
#include <SFML/Graphics.hpp>;

using namespace sf;


class Entity
{
public:

    Entity();
    virtual ~Entity() {}; 

    Vector2f position;
    Vector2f speed;
    float size;
    RectangleShape shape;

    void move(Vector2f direction);
};