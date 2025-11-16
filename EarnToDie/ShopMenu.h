#pragma once
#include <SFML/Graphics.hpp>
#include "const.h"
#include "AudioManager.h"
#include <vector>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class GameState;

class ShopMenu {
private:
    AudioManager& audioManager;
    sf::RenderWindow& gameWindow;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite background;

    std::vector<sf::Text> shopItems;
    std::vector<sf::Text> priceTexts;
    std::vector<sf::Text> levelTexts;
    std::vector<sf::RectangleShape> buttons;

    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color buttonColor;
    sf::Color buttonOutlineColor;
    sf::Color titleColor;
    sf::Color priceColor;

    bool isActive;
    int selectedIndex;
    int playerGold;
    int previousSelectedIndex;
    int& playerGoldRef;
    bool m_upgradesChanged = false;

    // Ñòàòèñòèêè óëó÷øåíèé
    struct Upgrade {
        std::string name;
        int basePrice;
        int currentLevel;
        int maxLevel;
        std::string description;
    };

    std::vector<Upgrade> upgrades;

    void initializeUpgrades();
    void initializeMenuItems();
    void initializeButtons();
    bool canAffordUpgrade(int index) const;
    int getUpgradePrice(int index) const;
    std::string getLevelText(int index) const;

public:
    ShopMenu(sf::RenderWindow& window, int& goldRef, AudioManager& audioManager);
    ~ShopMenu() = default;

    bool isActiveState() const { return isActive; }
    void setActive(bool active);
    void setPlayerGold(int gold) { playerGold = gold; }
    int getSelectedAction() const { return selectedIndex; }
    void resetSelection() { selectedIndex = -1; }

    void update();
    void render();
    void handleEvents();

    // Ãåòòåðû äëÿ óëó÷øåíèé (ïîíÿòíûå íàçâàíèÿ)
    int getFuelLevel() const {
        if (upgrades.size() > 0) return upgrades[0].currentLevel;
        return 0;
    }
    int getBoostLevel() const {
        if (upgrades.size() > 1) return upgrades[1].currentLevel;
        return 0;
    }
    int getSpeedLevel() const {
        if (upgrades.size() > 2) return upgrades[2].currentLevel;
        return 0;
    }

    int getFuelCapacity() const { return 100 + getFuelLevel() * 50; }
    int getBoostCapacity() const { return 100 + getBoostLevel() * 25; }
    float getSpeedMultiplier() const { return 1.0f + getSpeedLevel() * 0.2f; }

    // Ñîâìåñòèìîñòü ñî ñòàðûì êîäîì
    int getFuelCapacityLevel() const { return getFuelLevel(); }
    int getAccelerationLevel() const { return getBoostLevel(); }
    int getCarSpeedLevel() const { return getSpeedLevel(); }

    bool haveUpgradesChanged() const { return m_upgradesChanged; }
    void clearUpgradesChanged() { m_upgradesChanged = false; }
    void applyUpgradesImmediately();
    void resetUpgrades();

    // Çàãðóçêà/ñîõðàíåíèå óëó÷øåíèé
    void loadUpgrades();
    void saveUpgrades();

    void reloadUpgrades() {
        loadUpgrades();
    }

    enum ShopItems {
        FUEL_CAPACITY = 0,
        BOOST = 1,
        CAR_SPEED = 2,
        BACK = 3
    };
};