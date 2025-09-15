#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class GameState{
public:
	GameState() : playerHealth(100) {
		font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
		healthText.setFont(font);
		healthText.setCharacterSize(24);
		healthText.setFillColor(Color::White);
		healthText.setPosition(10, 10);

		updateHealthText();
	}
	void draw(RenderWindow& window);
	void decreaseHealth(int damage);
	void updateHealthText();

private:
	int playerHealth;
	Font font;
	Text healthText;
};

