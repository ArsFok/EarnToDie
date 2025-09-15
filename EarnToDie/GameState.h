#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class GameState{
public:
	GameState() : playerHealth(100), playerFuel(100) {
		font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
		healthText.setFont(font);
		healthText.setCharacterSize(24);
		healthText.setFillColor(Color::White);
		healthText.setPosition(10, 10);

		fuelText.setFont(font);
		fuelText.setCharacterSize(24);
		fuelText.setFillColor(Color::White);
		fuelText.setPosition(10, 40);

		updateHealthText();
		updateFuelText();
	}
	void draw(RenderWindow& window);
	void decreaseHealth(int damage);
	void updateHealthText();
	void decreaseFuel(int fuel);
	void updateFuelText();

private:
	int playerHealth;
	int playerFuel;
	Font font;
	Text healthText;
	Text fuelText;
};

