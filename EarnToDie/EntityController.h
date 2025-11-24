#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "const.h"
#include "GameState.h"
#include "ShopMenu.h"

using namespace sf;

class ShopMenu;

class EntityController
{
public:

    EntityController(GameState& gameState, ShopMenu* shopMenu = nullptr);
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

    bool hasPlayerWon() const { return m_hasPlayerWon; }
    void setPlayerWon(bool won) { m_hasPlayerWon = won; }

    bool isBoostActive() const { return m_isShiftPressed && m_canUseBoost; }

    void setShopMenu(ShopMenu* shopMenu) { m_shopMenu = shopMenu; applyShopUpgrades(); }
    void applyShopUpgrades();
    int getMaxBoostFuel() const { return m_maxBoostFuel; }
    float getBoostMultiplier() const { return m_boostMultiplier; }

private:
    bool m_isFinal = false;
    bool m_shouldRestart = false;
    bool m_returnToMainMenu = false;
    bool m_hasPlayerWon = false;

    bool m_canUseBoost = true;
    float m_boostFuel = 100;
    int m_maxBoostFuel = 100;
    float m_boostMultiplier = 2.0f;
    const float BOOST_FUEL_CONSUMPTION = 15.0f; 
    const float BOOST_FUEL_REGENERATION = 5.0f; 

    float m_baseGameSpeed = SPEED;
    float m_gameSpeed = SPEED;
    GameState& m_gameState;
    ShopMenu* m_shopMenu = nullptr;

    void checkInputs();
    void updateGameSpeed();
    void updateBoost(float deltaTime);

};