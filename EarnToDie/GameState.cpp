#include "GameState.h"
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;


void GameState::draw(RenderWindow& window) {
	window.draw(healthText);
}

void GameState::decreaseHealth(int damage) {
	playerHealth = playerHealth - damage;
	std::cout << "[decrease]" << playerHealth << std::endl;
	updateHealthText();
}

void GameState::updateHealthText() {
	stringstream ss;
	ss << "Health: " << playerHealth;
	healthText.setString(ss.str());
}