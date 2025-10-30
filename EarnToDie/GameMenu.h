#pragma once
#include <SFML/Graphics.hpp>
#include "MenuController.h"
#include "SettingsMenu.h"
#include <vector>
#include <string>

using namespace sf;
using namespace std;

class GameMenu {
public:
    GameMenu(RenderWindow& gameWindow, AudioManager& audioManager);
    ~GameMenu() = default;

    bool isActive() const { return isMenuActive; }
    void setActive(bool active) {
        isMenuActive = active;
        if (active) {
            menuController.resetSelection();
            updateMenuVisuals();
        }
    }
    int getMenuResult() const { return menuResult; }
    void resetMenu();

    bool isResetGoldRequested() const { return resetGoldRequested; }
    void clearResetGoldRequest() { resetGoldRequested = false; }
    void setTotalGold(int gold) { totalGoldValue = gold; }
    void resetGold() { resetGoldRequested = true; }
    bool isSettingsActive() const { return settingsMenu.isActive(); }
    SettingsMenu& getSettingsMenu() { return settingsMenu; }

    void update();
    void render();
    void handleEvents();

private:
    RenderWindow& gameWindow;
    Font font;
    vector<Text> menuItems;
    Texture backgroundTexture;
    Sprite background;

    MenuController menuController;
    SettingsMenu settingsMenu;

    Color normalColor;
    Color selectedColor;
    Color titleColor;
    Color backgroundColor;
    Color totalGold;

    bool isMenuActive;
    bool resetGoldRequested;
    int menuResult;
    int totalGoldValue;

    int previousSelectedIndex = -1;

    void initializeMenuItems();
    void updateMenuVisuals();
    void handleMenuSelection(int selectedIndex);
};

namespace MenuItems {
    const int START_GAME = 0;
    const int NEW_GAME = 1;
    const int SETTINGS = 2;
    const int SHOP = 3;
    const int EXIT = 4;
}