#include "GameState.h"
#include "EntityController.h"
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;



void GameState::draw(RenderWindow& window) {
	window.draw(speedText);
	window.draw(fuelText);
	window.draw(distText);
	window.draw(goldText);

	if (m_gameStatus != GameStatus::Playing) {
		window.draw(pauseOverlay);
		if (m_gameStatus == GameStatus::Paused) {
			window.draw(pauseText);
		}
		else if (m_gameStatus == GameStatus::GameOver) {
			window.draw(gameOverText);
		}
	}
}

void GameState::decreaseGold(int gold) {
	playerGold += gold;
	cout << "[decreaseG:]" << playerGold << endl;
	updateGoldText();
}
void GameState::decreaseSpeed(int speed) {
	playerSpeed = speed;
	cout << playerSpeed << endl;
	updateSpeedText();
}
void GameState::decreaseFuel(int fuel) {
	playerFuel -= fuel;
	if (playerFuel < 0) {
		playerFuel = 0;
		setGameOver();
	}
	cout << "[decreaseF:]" << playerFuel << endl;
	updateFuelText();
}
void GameState::decreaseDist(int dist) {
	playerDist += dist;
	if (dist > 1000) {
		setGameOver();
	}
	cout << "[decreaseD:]" << playerDist << endl;
	updateDistText();
}

void GameState::updateGoldText() {
	stringstream ss;
	ss << "Gold: " << playerGold;
	goldText.setString(ss.str());
}
void GameState::updateSpeedText() {
	stringstream ss;
	ss << "Speed: " << playerSpeed;
	speedText.setString(ss.str());
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
