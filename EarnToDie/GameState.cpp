#include "GameState.h"
#include "EntityController.h"
#include "ShopMenu.h"
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;

void GameState::draw(RenderWindow& window) {
	const float BLOCK_WIDTH = 190;
	const float BLOCK_HEIGHT = 180;
	const float PADDING = 15;

	RectangleShape statsBackground(Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
	statsBackground.setFillColor(Color(0, 0, 0, 180));
	statsBackground.setPosition(5, 5);
	window.draw(statsBackground);

	window.draw(speedText);    // (10, 10)
	window.draw(fuelText);     // (10, 40)  
	window.draw(distText);     // (10, 70)
	window.draw(goldText);     // (10, 100)
	window.draw(boostText);    // (10, 130)

	FloatRect helpBounds = helpText.getGlobalBounds();
	RectangleShape helpBackground(Vector2f(helpBounds.width + 40, helpBounds.height + 20));
	helpBackground.setFillColor(Color(0, 0, 0, 200));
	helpBackground.setPosition(helpBounds.left - 20, helpBounds.top - 10);
	window.draw(helpBackground);
	window.draw(helpText);
}

void GameState::decreaseSpeed(int speed) {
	playerSpeed = speed;
	updateSpeedText();
}

void GameState::setBoostInfo(int fuel, int maxFuel, bool isActive) {
	boostText.setFillColor(isActive ? Color::Yellow : Color::White);
	stringstream boostSS;
	boostSS << "Boost: " << fuel << "/" << maxFuel;
	if (isActive) boostSS << " \n[ACTIVE]";
	boostText.setString(boostSS.str());
}

void GameState::applyShopUpgrades() {
	if (!m_shopMenu) {
		std::cout << "DEBUG: No shop menu in GameState!" << std::endl;
		return;
	}

	std::cout << "DEBUG: GameState accessing shop menu at address: " << m_shopMenu << std::endl;
	std::cout << "DEBUG: Before getting fuel capacity from shop..." << std::endl;

	int oldMaxFuel = m_maxFuel;
	m_maxFuel = m_shopMenu->getFuelCapacity();

	std::cout << "=== FUEL UPGRADE APPLIED ===" << std::endl;
	std::cout << "Fuel Capacity: " << m_maxFuel << " (Level: " << m_shopMenu->getFuelLevel() << ")" << std::endl;
	std::cout << "Max Fuel: " << oldMaxFuel << " -> " << m_maxFuel << std::endl;
	std::cout << "Current Fuel: " << playerFuel << "/" << m_maxFuel << std::endl;
	std::cout << "============================" << std::endl;

	playerFuel = m_maxFuel;
	updateFuelText();
	updateSpeedText();
}

int GameState::getMaxFuel() const {
	return m_maxFuel;
}

void GameState::decreaseFuel(int fuel) {
	playerFuel -= fuel * 3;
	if (playerFuel <= 0) {
		playerFuel = 0;
		setGameOver();
	}
	cout << "[decreaseFUEL:]" << playerFuel << endl;
	updateFuelText();
}

void GameState::decreaseDist(int dist) {
	playerDist += dist;
	if (targetDistance > 0 && playerDist >= targetDistance) {
		cout << "[VICTORY!] Reached target: " << playerDist << "/" << targetDistance << endl;
		setGameWon();
	}
	else {
		cout << "[decreaseDIST:] " << playerDist;
		if (targetDistance > 0) {
			cout << "/" << targetDistance;
		}
		else {
			cout << " (no target set)";
		}
		cout << endl;
	}
	updateDistText();
}

void GameState::updateGoldText() {
	stringstream ss;
	ss << "Gold: " << playerGold;
	goldText.setString(ss.str());
}

void GameState::updateSpeedText() {
	stringstream ss;

	if (m_shopMenu) {
		int speedLevel = m_shopMenu->getCarSpeedLevel();
		float baseSpeed = playerSpeed * 40;
		float upgradedSpeed = baseSpeed * (1.0f + speedLevel * 0.2f);

		ss << "Speed: " << upgradedSpeed;
		speedText.setString(ss.str());
	}
	else {
		if (playerSpeed == 1) {
			ss << "Speed: " << playerSpeed * 30;
		}
		else {
			ss << "Speed: " << playerSpeed * 40;
		}
		speedText.setString(ss.str());
	}
}

void GameState::updateFuelText() {
	stringstream ss;
	ss << "Fuel: " << playerFuel << "/" << m_maxFuel;;
	fuelText.setString(ss.str());
}

void GameState::updateDistText() {
	stringstream ss;
	ss << "Distance: " << playerDist;
	if (targetDistance > 0) {
		ss << "/" << targetDistance;
	}
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

int GameState::getTotalGold() const {
	return totalGold;
}

void GameState::resetGold() {
	playerGold = 0;
	totalGold = 0;
	updateGoldText();
	saveGold();
	cout << "Gold reset to 0" << endl;
}


void GameState::addSlowTime(float additionalDuration) {
	if (slowEffect.isActive()) {
		float remaining = slowEffect.getRemainingTime();
		slowEffect.activate(remaining + additionalDuration, slowEffect.getSlowFactor());
	}
	else {
		slowEffect.activate(additionalDuration, 0.3f);
	}
}

bool GameState::isSlowEffectCritical() const {
	return slowEffect.getRemainingTime() > 100.0f;
}

void GameState::checkSlowEffectGameOver() {
	if (isSlowEffectCritical()) {
		setGameOver();
		std::cout << "GAME OVER: Slow effect reached critical duration (10 seconds)!" << std::endl;
	}
}


float GameState::getSpeedMultiplier() const {
	return slowEffect.isActive() ? slowEffect.getSlowFactor() : 1.0f;
}

bool GameState::isSlowed() const {
	return slowEffect.isActive();
}

float GameState::getSlowRemainingTime() const {
	return slowEffect.getRemainingTime();
}

void GameState::updateSlowEffect() {
	slowEffect.update();
}