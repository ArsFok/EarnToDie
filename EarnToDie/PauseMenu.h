#pragma once
#include "MenuController.h"
#include "SettingsMenu.h"
#include "const.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PauseMenu {
public:
    PauseMenu(sf::RenderWindow& gameWindow);
    ~PauseMenu() = default;

    void update();
    void render();
    void handleEvents();
    void updateMenuVisuals();

    sf::RenderWindow& getWindow() { return gameWindow; }
    void setActive(bool active);
    bool isActive() const { return isMenuActive; }

    int getSelectedAction() const { return menuController.getSelectedIndex(); }

    int getPauseMenuResult() { return pauseMenuResult; }
    void resetMenuResult() { pauseMenuResult = -1; }

    void setGamePaused(bool paused) { gamePaused = paused; }
    bool isGamePaused() const { return gamePaused; }

    bool isSettingsActive() const { return settingsMenu.isActive(); }
    SettingsMenu& getSettingsMenu() { return settingsMenu; }

private:
    sf::RenderWindow& gameWindow;
    sf::Font font;
    std::vector<sf::Text> menuPauseItems;
    std::vector<sf::RectangleShape> buttons;

    MenuController menuController;
    SettingsMenu settingsMenu;

    int pauseMenuResult = -1;
    int previousSelectedIndex = -1;
    bool isMenuActive = false;
    bool gamePaused = false;

    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color buttonColor;
    sf::Color buttonOutlineColor;

    void initializeMenuItems();
    void initializeButtons();
    void handleMenuSelection(int selectedIndex);
};
namespace PauseMenuItems {
	const int RESUME = 0;
	const int SETTINGS = 1;
	const int SHOP = 2;
	const int MAIN_MENU = 3;
	const int EXIT = 4;
}
