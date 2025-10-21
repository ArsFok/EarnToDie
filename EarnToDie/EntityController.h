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
	Entity* getEntity();

	bool isGamePaused() const { return m_isPaused; }
	bool shouldRestartGame() const { return m_shouldRestart; }
	bool isGameFinal() const { return m_isFinal; }

	void setPaused(bool paused) { m_isPaused = paused; }
	void resetRestartFlag() { m_shouldRestart = false; }
	void setFinal(bool final) { m_isFinal = final; }

	float getGameSpeed() const { return m_gameSpeed; }
	float getBaseGameSpeed() const { return m_baseGameSpeed; }

	void moveUp();
	void moveDown();
	int getSelectedMenuIndex() const { return m_selectedMenuIndex; }
	void resetMenuSelection() { m_selectedMenuIndex = 0; }

private:
	bool m_isPaused = false;
	bool m_isFinal = false;
	bool m_shouldRestart = false;

	float m_baseGameSpeed = SPEED;    // Базовая скорость (без ускорения)
	float m_gameSpeed = SPEED;        // Текущая скорость (с учетом ускорения)

	void checkInputs();
	void inputMove();
	void updateGameSpeed();

	//меню
	int m_selectedMenuIndex = 0;
	int m_maxMenuItems = 4;
};
