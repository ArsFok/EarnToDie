#pragma once
#include <SFML/Graphics.hpp>

class SlowEffect {
private:
    bool active;
    float duration;
    float slowFactor;
    sf::Clock timer;

    sf::RectangleShape effectOverlay;
    sf::Text effectText;
    sf::Font font;

public:
    SlowEffect();
    void activate(float duration, float factor);
    void update();
    void draw(sf::RenderWindow& window);
    bool isActive() const;
    float getSlowFactor() const;
    float getRemainingTime() const;
};