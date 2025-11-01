#include "GameState.h"
#include "EntityController.h"
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;



void GameState::draw(RenderWindow& window) {
	window.draw(speedText);
	window.draw(fuelText);
	window.draw(distText);
	window.draw(goldText);
}

void GameState::decreaseSpeed(int speed) {
	playerSpeed = speed;
	updateSpeedText();
}
void GameState::decreaseFuel(int fuel) {
	playerFuel -= fuel;
	if (playerFuel <= 0) {
		setGameOver();
	}
	cout << "[decreaseFUEL:]" << playerFuel << endl;
	updateFuelText();
}
void GameState::decreaseDist(int dist) {
	playerDist += dist;
	if (playerDist > 1000) {
		setGameWon();
	}
	cout << "[decreaseDIST:]" << playerDist << endl;
	updateDistText();
}

void GameState::updateGoldText() {
	stringstream ss;
	ss << "Gold: " << playerGold;
	goldText.setString(ss.str());
}
void GameState::updateSpeedText() {
	stringstream ss;
	if (playerSpeed == 1) {
		ss << "Speed: " << playerSpeed * 30;
		speedText.setString(ss.str());
	}
	else {
		ss << "Speed: " << playerSpeed * 40;
		speedText.setString(ss.str());
	}
}

void GameState::updateFuelText() {
	stringstream ss;
	ss << "Fuel: " << playerFuel;
	fuelText.setString(ss.str());
}

void GameState::updateDistText() {
	stringstream ss;
	ss << "Distation: " << playerDist << "/1000";
	distText.setString(ss.str());
}
void GameState::loadGold() {
	ifstream file("savegame.dat");
	if (file.is_open()) {
		file >> totalGold;
		file.close();
		cout << "Loaded gold: " << totalGold << endl;
	}
	else {
		totalGold = 0;
		cout << "No save file found, starting with 0 gold" << endl;
	}
	playerGold = 0;
}
void GameState::saveGold() {
	ofstream file("savegame.dat");
	if (file.is_open()) {
		file << totalGold;
		file.close();
		cout << "Saved gold: " << totalGold << endl;
	}
	else {
		cout << "Error saving gold!" << endl;
	}
}
void GameState::addGold(int amount) {
	playerGold += amount;
	totalGold += amount;
	cout << "[decreaseGOLD:]" << playerGold << endl;
	updateGoldText();

}
int GameState::getTotalGold() const{
	return totalGold;
}
void GameState::resetGold() {
	playerGold = 0;
	totalGold = 0;
	updateGoldText();
	saveGold();
	cout << "Gold reset to 0" << endl;
}