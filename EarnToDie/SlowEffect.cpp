#include "SlowEffect.h"
#include <iostream>
#include <cmath>

SlowEffect::SlowEffect() : active(false), duration(0.0f), slowFactor(1.0f), warningActive(false) {
    effectOverlay.setSize(sf::Vector2f(200, 60));
    effectOverlay.setFillColor(sf::Color(100, 100, 255, 150));
    effectOverlay.setOutlineColor(sf::Color::Blue);
    effectOverlay.setOutlineThickness(2);
    effectOverlay.setPosition(200, 10);

    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Failed to load font for slow effect" << std::endl;
    }

    effectText.setFont(font);
    effectText.setCharacterSize(20);
    effectText.setFillColor(sf::Color::White);
    effectText.setPosition(200, 20);

    warningText.setFont(font);
    warningText.setCharacterSize(30);
    warningText.setFillColor(sf::Color::Red);
    warningText.setStyle(sf::Text::Bold);
    warningText.setString("WARNING: CRITICAL SLOW!");
    warningText.setPosition(400, 50);
}

void SlowEffect::activate(float duration, float factor) {
    this->duration = duration;
    this->slowFactor = factor;
    this->active = true;
    this->warningActive = false;
    timer.restart();

    std::cout << "SLOW EFFECT: Activated for " << duration << " seconds, factor: " << factor << std::endl;
}

void SlowEffect::addSlowTime(float additionalDuration) {
    if (active) {
        float remaining = getRemainingTime();
        duration = remaining + additionalDuration;
        timer.restart();

        std::cout << "SLOW EFFECT: Added " << additionalDuration << " seconds. Total: " << duration << " seconds" << std::endl;
    }
    else {
        activate(additionalDuration, slowFactor);
    }
}

void SlowEffect::update() {
    if (active && timer.getElapsedTime().asSeconds() >= duration) {
        active = false;
        warningActive = false;
        std::cout << "SLOW EFFECT: Ended" << std::endl;
    }

    if (active) {
        float remaining = getRemainingTime();
        effectText.setString("SLOW: " + std::to_string(static_cast<int>(remaining)) + "s");

        if (remaining > 7.0f) {
            warningActive = true;
            effectOverlay.setFillColor(sf::Color(255, 50, 50, 200));
        }
        else if (remaining < 2.0f) {
            warningActive = false;
            int alpha = static_cast<int>(128 + 127 * sin(timer.getElapsedTime().asSeconds() * 10));
            effectOverlay.setFillColor(sf::Color(100, 100, 255, alpha));
        }
        else {
            warningActive = false;
            effectOverlay.setFillColor(sf::Color(100, 100, 255, 150));
        }
    }
    else {
        warningActive = false;
    }
}

void SlowEffect::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(effectOverlay);
        window.draw(effectText);

        if (warningActive) {
            window.draw(warningText);
        }
    }
}

bool SlowEffect::isActive() const {
    return active;
}

float SlowEffect::getSlowFactor() const {
    return slowFactor;
}

float SlowEffect::getRemainingTime() const {
    return active ? duration - timer.getElapsedTime().asSeconds() : 0.0f;
}