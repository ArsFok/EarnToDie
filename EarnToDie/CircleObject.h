#pragma once
#include <SFML/Graphics.hpp>
#include "StaticObject.h"
#include <iostream>

using namespace sf;

class CircleObject :public StaticObject {
public:
    CircleObject(float radius, Vector2f startPosition)
        : StaticObject(new CircleShape(radius), startPosition) {

        shape->setOrigin(radius / 2, radius / 2);

        texture = new Texture();
        if (texture->loadFromFile("Slime.png")) {
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
    ~CircleObject() {

    }
private:
    Texture* texture = nullptr;
};