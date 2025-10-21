#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "const.h"

using namespace sf;

class GameState{
public:
	enum class GameStatus{
		Playing,
		Paused,
		GameOver
	};
	GameState() : playerSpeed(0), playerFuel(FUEL), playerDist(0), playerGold(0), m_gameStatus(GameStatus::Playing) {
		font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
		speedText.setFont(font);
		speedText.setCharacterSize(24);
		speedText.setFillColor(Color::White);
		speedText.setPosition(10, 10);

		fuelText.setFont(font);
		fuelText.setCharacterSize(24);
		fuelText.setFillColor(Color::White);
		fuelText.setPosition(10, 40);

		distText.setFont(font);
		distText.setCharacterSize(24);
		distText.setFillColor(Color::White);
		distText.setPosition(10, 70);

		goldText.setFont(font);
		goldText.setCharacterSize(24);
		goldText.setFillColor(Color::White);
		goldText.setPosition(10, 100);

		pauseText.setFont(font);
		pauseText.setString("PAUSED\nPress ESC to continue");
		pauseText.setCharacterSize(40);
		pauseText.setFillColor(Color::Red);
		pauseText.setStyle(Text::Bold);
		pauseText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50);

		gameOverText.setFont(font);
		gameOverText.setString("GAME OVER\nPress ENTER to restart");
		gameOverText.setCharacterSize(40);
		gameOverText.setFillColor(Color::Red);
		gameOverText.setStyle(Text::Bold);
		gameOverText.setPosition(WINDOW_WIDTH / 2 - 180, WINDOW_HEIGHT / 2 - 50);

		pauseOverlay.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
		pauseOverlay.setFillColor(Color(0, 0, 0, 150));

		updateSpeedText();
		updateFuelText();
		updateDistText();
		updateGoldText();
	}
	void draw(RenderWindow& window);
	void updateGoldText();
	void decreaseSpeed(int damage);
	void updateSpeedText();
	void decreaseFuel(int fuel);
	void updateFuelText();
	void decreaseDist(int dist);
	void updateDistText();

	bool isPaused() const { return m_gameStatus == GameStatus::Paused; }
	bool isGameOver() const { return m_gameStatus == GameStatus::GameOver; }
	bool isPlaying() const { return m_gameStatus == GameStatus::Playing; }

    void setPaused(bool paused) { m_gameStatus = paused ? GameStatus::Paused : GameStatus::Playing; }
	void setGameOver() { m_gameStatus = GameStatus::GameOver; }
	void restartGame() {
		m_gameStatus = GameStatus::Playing;
		playerSpeed = 0;
		playerFuel = FUEL;
		playerGold = 0;
		playerDist = 0;
		updateSpeedText();
		updateFuelText();
		updateDistText();
		updateGoldText();
	}
	GameStatus getGameStatus() const { return m_gameStatus; }

	void loadGold();
	void saveGold();
	void addGold(int amount);
	int getTotalGold() const;

private:
	float playerSpeed;	// Скорость  
	int playerFuel;		// Топливо 
	int playerDist;		// Расстояние
	int playerGold;		// Золото
	int totalGold;		// Общее золото за все время
	GameStatus m_gameStatus;

	Font font;
	Text speedText;
	Text fuelText;
	Text distText;
	Text goldText;

	Text pauseText;
	Text gameOverText;
	RectangleShape pauseOverlay;
};
