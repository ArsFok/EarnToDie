#include "MovingObject.h"

void MovingObject::update(Vector2f moveDirection) {
    shape->move(moveDirection);
}

FloatRect MovingObject::getGlobalBounds() const {
    return shape->getGlobalBounds();
}

void MovingObject::setColor(const Color& color) {
    currentColor = color;
    shape->setFillColor(color);
}

void MovingObject::setRotation(float angle) {
    currentRotation = angle;
    shape->setRotation(angle);
}

Color MovingObject::getColor() const {
    return currentColor;
}
bool MovingObject::checkCollision(const sf::Shape& other) const {
    return shape->getGlobalBounds().intersects(other.getGlobalBounds());
}