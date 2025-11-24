#include "EntityController.h"
#include <iostream>
#include "Entity.h"
#include "GameState.h"
#include "ShopMenu.h" 

EntityController::EntityController(GameState& gameState, ShopMenu* shopMenu)
    : m_gameState(gameState), m_shopMenu(shopMenu), m_baseGameSpeed(1.0f), m_gameSpeed(1.0f)
{
    entity = new Entity();
    applyShopUpgrades();
}

void EntityController::update(RenderWindow& window)
{
    static Clock deltaClock;
    float deltaTime = deltaClock.restart().asSeconds();

    checkInputs();
    updateBoost(deltaTime);
    window.draw(entity->shape);
}

Entity* EntityController::getEntity()
{
    return entity;
}

void EntityController::checkInputs()
{
    inputMove();
}

void EntityController::inputMove()
{
    static Clock inputCooldown;
    const Time cooldownTime = milliseconds(150);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && inputCooldown.getElapsedTime() > cooldownTime) {
        m_shouldRestart = true;
        m_isFinal = false;
        inputCooldown.restart();
    }
    bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
    if (shiftPressed != m_isShiftPressed) {
        m_isShiftPressed = shiftPressed;
        updateGameSpeed();
    }
    Vector2f direction;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x = 1;
    }
    else {
        direction.x = 0;
    }
    if (direction.x == 0) {
        return;
    }
    entity->move(direction);
}

void EntityController::updateGameSpeed() {
    if (m_isShiftPressed && m_canUseBoost) {
        m_gameSpeed = m_baseGameSpeed * m_boostMultiplier;
    }
    else {
        m_gameSpeed = m_baseGameSpeed;
        if (m_isShiftPressed) {
            std::cout << "Boost disabled - no fuel" << std::endl;
        }
    }
}

void EntityController::updateBoost(float deltaTime)
{
    if (m_isShiftPressed && m_canUseBoost) {
        float consumption = BOOST_FUEL_CONSUMPTION * deltaTime;
        if (m_shopMenu) {
            int boostLevel = m_shopMenu->getBoostLevel();
            consumption *= (1.0f - boostLevel * 0.1f);
        }
        if (consumption > m_boostFuel) {
            consumption = m_boostFuel;
        }

        m_boostFuel -= consumption;

        if (m_boostFuel <= 0) {
            m_boostFuel = 0;
            m_canUseBoost = false;
            m_isShiftPressed = false;
            updateGameSpeed();
            std::cout << "Boost fuel depleted!" << std::endl;
        }
    }
    else {
        float regeneration = BOOST_FUEL_REGENERATION * deltaTime;
        if (m_shopMenu) {
            int boostLevel = m_shopMenu->getBoostLevel();
            regeneration *= (1.0f + boostLevel * 0.15f);
        }
        if (m_boostFuel < m_maxBoostFuel) {
            m_boostFuel += regeneration;
            if (m_boostFuel > m_maxBoostFuel) {
                m_boostFuel = m_maxBoostFuel;
            }

            if (m_boostFuel >= 20 && !m_canUseBoost) {
                m_canUseBoost = true;
                std::cout << "Boost available again!" << std::endl;
            }
        }
    }
    m_gameState.setBoostInfo(m_boostFuel, m_maxBoostFuel, (m_isShiftPressed && m_canUseBoost && m_boostFuel > 0));
}

void EntityController::applyShopUpgrades() {
    if (!m_shopMenu) {
        std::cout << "DEBUG: No shop menu connected!" << std::endl;
        return;
    }

    std::cout << "DEBUG: EntityController accessing shop menu at address: " << m_shopMenu << std::endl;
    std::cout << "DEBUG: Before getting levels from shop..." << std::endl;
    int speedLevel = m_shopMenu->getSpeedLevel();
    int boostLevel = m_shopMenu->getBoostLevel();

    std::cout << "DEBUG: EntityController - Speed level: " << speedLevel
        << ", Boost level: " << boostLevel << std::endl;

    float oldBaseSpeed = m_baseGameSpeed;
    float oldBoostMultiplier = m_boostMultiplier;
    int oldMaxBoostFuel = m_maxBoostFuel;

    m_baseGameSpeed = SPEED * (1.0f + speedLevel * 0.2f);
    m_boostMultiplier = 2.0f + boostLevel * 0.1f;
    m_maxBoostFuel = 100 + boostLevel * 20;

    std::cout << "=== ENTITY UPGRADES APPLIED ===" << std::endl;
    std::cout << "Base Speed: " << oldBaseSpeed << " -> " << m_baseGameSpeed << std::endl;
    std::cout << "Boost Multiplier: " << oldBoostMultiplier << " -> " << m_boostMultiplier << std::endl;
    std::cout << "Max Boost Fuel: " << oldMaxBoostFuel << " -> " << m_maxBoostFuel << std::endl;
    std::cout << "===============================" << std::endl;

    updateGameSpeed();
}

EntityController::~EntityController() {
    delete entity;
}