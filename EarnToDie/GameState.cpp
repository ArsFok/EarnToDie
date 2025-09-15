#include "GameState.h"
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;


void GameState::draw(RenderWindow& window) {
	window.draw(healthText);
	window.draw(fuelText);
}

void GameState::decreaseHealth(int damage) {
	playerHealth -= damage;
	cout << "[decreaseH:]" << playerHealth << endl;
	updateHealthText();
}
void GameState::decreaseFuel(int fuel) {
	playerFuel -= fuel;
	cout << "[decreaseF:]" << playerFuel << endl;
	updateFuelText();
}


void GameState::updateHealthText() {
	stringstream ss;
	ss << "Health: " << playerHealth;
	healthText.setString(ss.str());
}

void GameState::updateFuelText() {
	stringstream ss;
	ss << "Fuel: " << playerFuel;
	fuelText.setString(ss.str());
}