#pragma once
#include <SFML/Graphics.hpp>
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

    bool shouldRestartGame() const { return m_shouldRestart; }
    void resetRestartFlag() { m_shouldRestart = false; }
    void setFinal(bool final) { m_isFinal = final; }

    float getGameSpeed() const { return m_gameSpeed; }
    float getBaseGameSpeed() const { return m_baseGameSpeed; }

    bool shouldReturnToMainMenu() const { return m_returnToMainMenu; }
    void setReturnToMainMenu() { m_returnToMainMenu = true; }
    void resetReturnToMainMenu() { m_returnToMainMenu = false; }

    bool isGameFinal() const { return m_isFinal; }

    // ƒобавленный метод дл€ проверки победы игрока
    bool hasPlayerWon() const { return m_hasPlayerWon; }
    void setPlayerWon(bool won) { m_hasPlayerWon = won; }

private:
    bool m_isFinal = false;
    bool m_shouldRestart = false;
    bool m_returnToMainMenu = false;
    bool m_hasPlayerWon = false;  // ƒобавлена переменна€ дл€ отслеживани€ победы

    float m_baseGameSpeed = SPEED;    // Ѕазова€ скорость (без ускорени€)
    float m_gameSpeed = SPEED;        // “екуща€ скорость (с учетом ускорени€)

    void checkInputs();
    void updateGameSpeed();

};