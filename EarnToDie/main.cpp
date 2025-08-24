#include <SFML/Graphics.hpp>
#include "EntityController.h"
    
using namespace sf;
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "EarnToDie");

    EntityController controller;
    while (window.isOpen()) {
        window.clear();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        controller.update(window);
        window.display();
    }
    return 0;
}