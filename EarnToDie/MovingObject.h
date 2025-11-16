#pragma once
#include <SFML/Graphics.hpp>

class MovingObject {
public:
    MovingObject(sf::Shape* shape, sf::Vector2f startPosition, int damage)
        : shape(shape), damage(damage) {
        shape->setPosition(startPosition);
    }

    virtual ~MovingObject() {
        delete shape;
    }

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0;
    virtual bool checkCollision(const sf::Shape& other) const = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual void update(const sf::Vector2f& direction) = 0;
    virtual void setColor(const sf::Color& color) = 0;
    virtual void setRotation(float angle) = 0;

    sf::Shape* getShape() { return shape; }

protected:
    sf::Shape* shape;
    int damage;
};