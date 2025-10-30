#pragma once
#include "MenuController.h"
#include "const.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>

class SettingsMenu {
public:
    SettingsMenu(sf::RenderWindow& gameWindow);
    ~SettingsMenu() = default;

    void update();
    void render();
    void handleEvents();

    sf::RenderWindow& getWindow() { return gameWindow; }
    void setActive(bool active);
    bool isActive() const { return isMenuActive; }

    int getSelectedAction() const { return menuController.getSelectedIndex(); }
    int getSettingsMenuResult() { return settingsMenuResult; }
    void resetMenuResult() { settingsMenuResult = -1; }

    // Геттеры для настроек
    float getMusicVolume() const { return musicVolume; }
    float getSoundVolume() const { return soundVolume; }

private:
    sf::RenderWindow& gameWindow;
    sf::Texture backgroundTexture;
    sf::Sprite background;

    sf::Font font;
    std::vector<sf::Text> menuSettingsItems;
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> valueTexts;

    MenuController menuController;

    int settingsMenuResult = -1;
    int previousSelectedIndex = -1;
    bool isMenuActive = false;

    // Настройки
    float musicVolume = 50.0f;
    float soundVolume = 70.0f;

    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color buttonColor;
    sf::Color buttonOutlineColor;
    sf::Color valueColor;

    void initializeMenuItems();
    void initializeButtons();
    void updateMenuVisuals();
    void handleMenuSelection(int selectedIndex);
    void updateValueTexts();
    void adjustSetting(int direction);
    void handleContinuousInput();

    void saveSettings();
    void loadSettings();
    std::string getSettingsFilePath();
};

namespace SettingsMenuItems {
    const int MUSIC_VOLUME = 0;
    const int SOUND_VOLUME = 1;
    const int SAVE_SETTINGS = 2;
    const int BACK = 3;
}