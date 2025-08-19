#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
 
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

    sf::Texture boxTexture;
    if (!boxTexture.loadFromFile("box.jpg")) {
        return -1;
    }
    sf::Sprite boxSprite;
    boxSprite.setTexture(boxTexture);
    boxSprite.setPosition(
        std::rand() % (800 - boxTexture.getSize().x),
        std::rand() % (600 - boxTexture.getSize().y)
    );
    sf::Vector2f direction(
        (std::rand() % 5) + 1,
        (std::rand() % 5) + 1
    );

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        boxSprite.move(direction);

        sf::Vector2f position = boxSprite.getPosition();
        sf::Vector2u textureSize = boxTexture.getSize();

        if (position.x <= 0 || position.x >= 800 - textureSize.x) {
            direction.x = -direction.x;
            direction.y += (std::rand() % 3) - 1;
        }

        if (position.y <= 0 || position.y >= 600 - textureSize.y) {
            direction.y = -direction.y;
            direction.x += (std::rand() % 3) - 1;
        }

        window.clear();
        window.draw(boxSprite);
        window.display();

        sf::sleep(sf::milliseconds(16));
    }

    return 0;
}
