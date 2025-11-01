#pragma once
#include <SFML/Graphics.hpp>
#include "StaticObject.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace sf;

class CircleObject : public StaticObject {
public:
    CircleObject(float radius, Vector2f startPosition)
        : StaticObject(new CircleShape(radius), startPosition) {

        shape->setOrigin(radius / 2, radius / 2);

        // Список доступных текстур
        std::vector<std::string> textureFiles = {
            "Slime.png",
            "Trapdoor.png",
        };

        // Выбираем случайную текстуру
        int randomIndex = rand() % textureFiles.size();
        std::string selectedTexture = textureFiles[randomIndex];

        texture = new Texture();
        if (texture->loadFromFile(selectedTexture)) {
            shape->setTexture(texture);
            shape->setFillColor(Color::White);
        }
        else {
            std::cout << "Failed to load " << selectedTexture << "! Using red color." << std::endl;
            shape->setFillColor(Color::Red);
            delete texture;
            texture = nullptr;
        }
    }

    ~CircleObject() {
        if (texture) {
            delete texture;
        }
    }

private:
    Texture* texture = nullptr;
};