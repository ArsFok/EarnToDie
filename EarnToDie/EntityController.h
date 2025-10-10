#pragma once
#include <SFML/Graphics.hpp>;
#include "Entity.h"

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
	bool isGameFinal() const { return m_isFinal; }
	void setPaused(bool paused) { m_isPaused = paused; }
	void setFinal(bool final) { m_isFinal = final; }
	bool shouldRestartGame() const { return m_shouldRestart; }
	void resetRestartFlag() { m_shouldRestart = false; }

	float getGameSpeed() const { return m_gameSpeed; }
	float getBaseGameSpeed() const { return m_baseGameSpeed; }

private:
	bool m_isPaused = false;
	bool m_isFinal = false;
	bool m_shouldRestart = false;

	float m_baseGameSpeed = 1.0f;    // Базовая скорость (без ускорения)
	float m_gameSpeed = 1.0f;        // Текущая скорость (с учетом ускорения)

	void checkInputs();
	void inputMove();
	void updateGameSpeed();
};
