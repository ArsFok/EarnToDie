#include <SFML/Graphics.hpp>

class Box {
public:
    sf::Vector2f position;
    sf::Vector2f speed;
    float size;
};

void update(Box& box) {
    
    box.position += box.speed;

    float halh_size = box.size / 2;
    
    if (box.position.x - halh_size < 0) {
        box.position.x = halh_size;
    }
    else if (box.position.x > 800) {
        box.position.x = 800 - halh_size;
    }
    
    if (box.position.y - halh_size < 0) {
        box.position.y = halh_size;
    }
    else if (box.position.y + halh_size > 600) {
        box.position.y = 600 - halh_size;
    }
}
void keyboards (Box& box) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        box.speed.x = -0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        box.speed.x = 0.1f;
    }
    else {
        box.speed.x = 0.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        box.speed.y = -0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        box.speed.y = 0.1f;
    }
    else {
        box.speed.y = 0.f;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My SFML");

    Box box;
    box.position = sf::Vector2f(400.f, 300.f);
    box.speed = sf::Vector2f(0.f, 0.f);
    box.size = 100.f;

    sf::RectangleShape kub(sf::Vector2f(box.size, box.size));
    kub.setOrigin(box.size / 2, box.size / 2);
    kub.setFillColor(sf::Color::Green);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        keyboards(box);
        update(box);

        kub.setPosition(box.position);

        window.clear();
        window.draw(kub);
        window.display();
    }

    return 0;
}
