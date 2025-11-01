#pragma once
#include <SFML/Graphics.hpp>
#include "const.h"
#include <vector>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class ShopMenu {
private:

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

    // Статистики улучшений
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
    ShopMenu(sf::RenderWindow& window, int& goldRef);
    ~ShopMenu() = default;

    bool isActiveState() const { return isActive; }
    void setActive(bool active);
    void setPlayerGold(int gold) { playerGold = gold; }
    int getSelectedAction() const { return selectedIndex; }
    void resetSelection() { selectedIndex = -1; }


    void update();
    void render();
    void handleEvents();

    // Геттеры для улучшений
    int getFuelCapacity() const { return 100 + upgrades[0].currentLevel * 50; } // Базовый бак 100 + 50 за уровень
    int getAccelerationLevel() const { return upgrades[1].currentLevel; } // Уровень ускорения (0-5)
    int getCarSpeedLevel() const { return upgrades[2].currentLevel; } // Уровень скорости машины (0-5)

    // Загрузка/сохранение улучшений
    void loadUpgrades();
    void saveUpgrades();

    void resetUpgrades() {
        for (auto& upgrade : upgrades) {
            upgrade.currentLevel = 0;
        }
        saveUpgrades(); // Сохраняем сброшенное состояние
        std::cout << "All shop upgrades reset to 0" << std::endl;
    }


    enum ShopItems {
        FUEL_CAPACITY = 0,
        ACCELERATION = 1,
        CAR_SPEED = 2,
        BACK = 3
    };
};