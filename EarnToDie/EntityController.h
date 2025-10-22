#pragma once
#include <SFML/Graphics.hpp>;
#include "Entity.h"
#include "const.h"

using namespace sf;

class EntityController
{
public:

	EntityController();
	~EntityController();

	Entity* entity;
	bool m_isShiftPressed = false;

	void update(RenderWindow& window);
	void inputMove();
	Entity* getEntity();

	bool isGamePaused() const { return m_isPaused; }
	bool shouldRestartGame() const { return m_shouldRestart; }
	bool isGameFinal() const { return m_isFinal; }

	void setPaused(bool paused) { m_isPaused = paused; }
	void resetRestartFlag() { m_shouldRestart = false; }
	void setFinal(bool final) { m_isFinal = final; }

	float getGameSpeed() const { return m_gameSpeed; }
	float getBaseGameSpeed() const { return m_baseGameSpeed; }

	int getSelectedMenuIndex() const { return m_selectedMenuIndex; }
	int getSelectedPauseMenuIndex() const { return m_selectedPauseMenuIndex; }
	void resetMenuSelection() { m_selectedMenuIndex = 0; }

	bool shouldReturnToMainMenu() const { return m_returnToMainMenu; }
	void setReturnToMainMenu() { m_returnToMainMenu = true; }
	void resetReturnToMainMenu() { m_returnToMainMenu = false; }

	void moveMainMenuUp() { m_selectedMenuIndex = std::max(0, m_selectedMenuIndex - 1); }
	void moveMainMenuDown() { m_selectedMenuIndex = std::min(m_maxMenuItems - 1, m_selectedMenuIndex + 1); }
	void movePauseMenuUp() { m_selectedPauseMenuIndex = std::max(0, m_selectedPauseMenuIndex - 1); }
	void movePauseMenuDown() { m_selectedPauseMenuIndex = std::min(m_maxPauseMenuItems - 1, m_selectedPauseMenuIndex + 1); }

private:
	bool m_isPaused = false;
	bool m_isFinal = false;
	bool m_shouldRestart = false;
	bool m_returnToMainMenu = false;

	float m_baseGameSpeed = SPEED;    // Базовая скорость (без ускорения)
	float m_gameSpeed = SPEED;        // Текущая скорость (с учетом ускорения)

	void checkInputs();
	void updateGameSpeed();

	//меню
	int m_selectedMenuIndex = 0;
	int m_selectedPauseMenuIndex = 0;
	int m_maxPauseMenuItems = 5;
	int m_maxMenuItems = 4;
};
