#include <SFML/Graphics.hpp>

class Entity{
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

void keyboards (Entity& entity) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        entity.speed.x = -0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        entity.speed.x = 0.1f;
    }
    else {
        entity.speed.x = 0.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        entity.speed.y = -0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        entity.speed.y = 0.1f;
    }
    else {
        entity.speed.y = 0.f;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My SFML");

    Entity entity;
    entity.position = sf::Vector2f(400.f, 300.f);
    entity.speed = sf::Vector2f(0.f, 0.f);
    entity.size = 100.f;

    sf::RectangleShape kub(sf::Vector2f(entity.size, entity.size));
    kub.setOrigin(entity.size / 2, entity.size / 2);
    kub.setFillColor(sf::Color::Green);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        keyboards(entity);

        kub.setPosition(entity.position);

        window.clear();
        window.draw(kub);
        window.display();
    }

    return 0;
}
