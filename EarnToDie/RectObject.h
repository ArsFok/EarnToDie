#pragma once
#include <SFML/Graphics.hpp>
#include "MovingObject.h"
#include <iostream>

using namespace sf;

class RectObject : public MovingObject {
public:
    RectObject(float size, Vector2f startPosition, int damage)
        : MovingObject(new RectangleShape(Vector2f(size, size)), startPosition, damage) {

        shape->setOrigin(size / 2, size / 2);

        texture = new Texture();
        if (texture->loadFromFile("box.png")) {
            shape->setTexture(texture);
            shape->setFillColor(Color::White);
        }
        else {
            std::cout << "Failed to load box.png! Using red color." << std::endl;
            shape->setFillColor(Color::Red);
            delete texture;
            texture = nullptr;
        }
    }
    ~RectObject() {

    }
private:
    Texture* texture = nullptr;
};