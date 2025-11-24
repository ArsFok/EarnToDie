#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "const.h"
#include "SlowEffect.h"

using namespace sf;
using namespace std;

class ShopMenu;

class GameState {
public:
    enum class GameStatus {
        Playing,
        Paused,
        GameOver,
        GameWon
    };

    GameState() : playerSpeed(0), playerBoostFuel(100), playerFuel(FUEL), playerDist(0), playerGold(0), totalGold(0), m_gameStatus(GameStatus::Playing), m_shopMenu(nullptr) {
        font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

        speedText.setFont(font);
        speedText.setCharacterSize(24);
        speedText.setFillColor(Color::White);
        speedText.setPosition(10, 10);

        boostText.setFont(font);
        boostText.setCharacterSize(24);
        boostText.setFillColor(Color::White);
        boostText.setPosition(10, 130);

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

        helpText.setFont(font);
        helpText.setCharacterSize(18);
        helpText.setFillColor(Color::White);
        helpText.setStyle(Text::Bold);
        helpText.setString("CONTROLS: ARROWS/A,D - Move  |  SHIFT - Boost  |  ESC - Pause");

        FloatRect textBounds = helpText.getLocalBounds();
        helpText.setPosition((WINDOW_WIDTH - textBounds.width) / 2, WINDOW_HEIGHT - 40);

        updateSpeedText();
        updateFuelText();
        updateDistText();
        updateGoldText();
    }

    void setShopMenu(ShopMenu* shopMenu) {
        m_shopMenu = shopMenu;
        applyShopUpgrades();
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
    bool isGameWon() const { return m_gameStatus == GameStatus::GameWon; }
    bool isPlaying() const { return m_gameStatus == GameStatus::Playing; }
    bool isTargetSet() const { return targetDistance > 0; }

    void setPaused(bool paused) { m_gameStatus = paused ? GameStatus::Paused : GameStatus::Playing; }
    void setGameOver() { m_gameStatus = GameStatus::GameOver; }
    void setGameWon() { m_gameStatus = GameStatus::GameWon; }

    void resetDistance() {
        playerDist = 0;
        targetDistance = 0;
        updateDistText();
        cout << "DEBUG: Distance reset to " << playerDist << endl;
    }

    void setTargetDistance(int distance) { targetDistance = distance; }
    int getTargetDistance() const { return targetDistance; }

    void restartGame() {
        m_gameStatus = GameStatus::Playing;
        playerSpeed = 0;
        playerFuel = getMaxFuel();
        playerGold = 0;
        playerDist = 0;
        targetDistance = 0;
        updateSpeedText();
        updateFuelText();
        updateDistText();
        updateGoldText();
    }

    void resetAll() {
        m_gameStatus = GameStatus::Playing;
        totalGold = 0;
        playerGold = 0;
        playerDist = 0;
        targetDistance = 0;
        playerFuel = getMaxFuel();
        saveGold();
        updateGoldText();
        updateDistText();
        updateFuelText();
        cout << "DEBUG: GameState completely reset" << endl;
    }

    GameStatus getGameStatus() const { return m_gameStatus; }

    void loadGold();
    void saveGold();
    void addGold(int amount);
    void resetGold();
    int getTotalGold() const;

    float getPlayerSpeed() const { return playerSpeed; }
    int getPlayerFuel() const { return playerFuel; }
    int getPlayerDist() const { return playerDist; }
    int getPlayerGold() const { return playerGold; }
    int& getTotalGoldRef() { return totalGold; }

    void setBoostInfo(int fuel, int maxFuel, bool isActive);

    void applyShopUpgrades();
    int getMaxFuel() const;

    void addSlowTime(float additionalDuration);
    bool isSlowEffectCritical() const;
    void checkSlowEffectGameOver();

    float getSpeedMultiplier() const;
    bool isSlowed() const;
    float getSlowRemainingTime() const;
    void updateSlowEffect();

    void drawSlowEffect(sf::RenderWindow& window) {
        slowEffect.draw(window);
    }

private:
    float playerSpeed;
    int playerBoostFuel;
    int MaxPlayerBoostFuel;
    int playerFuel;
    int playerDist;
    int targetDistance;
    int playerGold;
    int totalGold;
    bool boostActive = false;

    GameStatus m_gameStatus;
    SlowEffect slowEffect;
    ShopMenu* m_shopMenu;

    Font font;
    Text speedText;
    Text fuelText;
    Text distText;
    Text goldText;
    Text boostText;
    Text helpText;

    int m_maxFuel = 100;
};