#pragma once

class Entity {
public:
    sf::Vector2f position;
    sf::Vector2f speed;
    float size;

    void move() {
        position += speed;

        float halh_size = size / 2;

        if (position.x - halh_size < 0) {
            position.x = halh_size;
        }
        else if (position.x + halh_size > 800) {
            position.x = 800 - halh_size;
        }

        if (position.y - halh_size < 0) {
            position.y = halh_size;
        }
        else if (position.y + halh_size > 600) {
            position.y = 600 - halh_size;
        }
    }
};
