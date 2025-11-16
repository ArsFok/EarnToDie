#include "SlowEffect.h"
#include <iostream>

SlowEffect::SlowEffect() : active(false), duration(0.0f), slowFactor(1.0f) {
    // Настройка визуального эффекта
    effectOverlay.setSize(sf::Vector2f(200, 60));
    effectOverlay.setFillColor(sf::Color(100, 100, 255, 150)); // Синий полупрозрачный
    effectOverlay.setOutlineColor(sf::Color::Blue);
    effectOverlay.setOutlineThickness(2);
    effectOverlay.setPosition(200, 10);

    // Загрузка шрифта (добавьте файл шрифта в проект)
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Failed to load font for slow effect" << std::endl;
    }

    effectText.setFont(font);
    effectText.setCharacterSize(20);
    effectText.setFillColor(sf::Color::White);
    effectText.setPosition(200, 20);
}

void SlowEffect::activate(float duration, float factor) {
    this->duration = duration;
    this->slowFactor = factor;
    this->active = true;
    timer.restart();

    std::cout << "SLOW EFFECT: Activated for " << duration << " seconds, factor: " << factor << std::endl;
}

void SlowEffect::update() {
    if (active && timer.getElapsedTime().asSeconds() >= duration) {
        active = false;
        std::cout << "SLOW EFFECT: Ended" << std::endl;
    }

    // Обновление текста
    if (active) {
        float remaining = getRemainingTime();
        effectText.setString("SLOW: " + std::to_string(static_cast<int>(remaining)) + "s");

        // Мигание эффекта при окончании
        if (remaining < 2.0f) {
            int alpha = static_cast<int>(128 + 127 * sin(timer.getElapsedTime().asSeconds() * 10));
            effectOverlay.setFillColor(sf::Color(100, 100, 255, alpha));
        }
    }
}

void SlowEffect::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(effectOverlay);
        window.draw(effectText);
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