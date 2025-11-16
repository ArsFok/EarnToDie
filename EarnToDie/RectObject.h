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
        if (texture->loadFromFile("zombie.png")) {
            shape->setTexture(texture);
            shape->setFillColor(Color::White);
        }
        else {
            std::cout << "Failed to load zombie.png! Using red color." << std::endl;
            shape->setFillColor(Color::Red);
            delete texture;
            texture = nullptr;
        }
    }

    ~RectObject() {
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(*shape);
    }

    sf::FloatRect getGlobalBounds() const override {
        return shape->getGlobalBounds();
    }

    bool checkCollision(const sf::Shape& other) const override {
        return shape->getGlobalBounds().intersects(other.getGlobalBounds());
    }

    sf::Vector2f getPosition() const override {
        return shape->getPosition();
    }

    void setPosition(float x, float y) override {
        shape->setPosition(x, y);
    }

    void update(const sf::Vector2f& direction) override {
        shape->move(direction);
    }

    void setColor(const sf::Color& color) override {
        shape->setFillColor(color);
    }

    void setRotation(float angle) override {
        shape->setRotation(angle);
    }

private:
    Texture* texture = nullptr;
};