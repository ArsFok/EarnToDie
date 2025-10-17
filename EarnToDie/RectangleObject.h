#pragma once
#include <SFML/Graphics.hpp>
#include "MovingObject.h"
#include <iostream>

using namespace sf;

class RectangleObject : public MovingObject {
public:
    RectangleObject(float size_a, float size_b, Vector2f startPosition, int damage)
        : MovingObject(new RectangleShape(Vector2f(size_a, size_b)), startPosition, damage) {

        shape->setOrigin(size_a / 2, size_b / 2);

        std::vector<std::string> textureFiles = {
            "zombie.png",
            "zombie_v2.png"
        };

        std::string randomTexture = textureFiles[rand() % textureFiles.size()];

        texture = new Texture();
        if (texture->loadFromFile(randomTexture)) {
            shape->setTexture(texture);
            shape->setFillColor(Color::White);
        }
        else {
            std::cout << "Failed to load " << randomTexture << "! Using red color." << std::endl;
            shape->setFillColor(Color::Red);
            delete texture;
            texture = nullptr;
        }
    }
    ~RectangleObject() {

    }
private:
    Texture* texture = nullptr;
};