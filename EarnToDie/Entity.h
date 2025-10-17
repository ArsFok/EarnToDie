#pragma once
#include <SFML/Graphics.hpp>;
#include "const.h"

using namespace sf;


class Entity
{
private:
    float size;
    Vector2f speed;
    Texture carTexture;

public:
    Entity();
    virtual ~Entity() {};

    RectangleShape shape;
    void move(Vector2f direction);
};