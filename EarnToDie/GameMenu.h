#pragma once
#include <SFML/Graphics.hpp>
#include "MenuController.h"
#include "SettingsMenu.h"
#include "LevelMenu.h"
#include "ShopMenu.h"
#include <vector>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class GameMenu {
public:
    GameMenu(RenderWindow& gameWindow, AudioManager& audioManager, int& goldRef, ShopMenu& shopMenu, LevelMenu& levelMenu)
        : gameWindow(gameWindow)
        , menuController(5)
        , settingsMenu(gameWindow, audioManager)
        , levelMenu(levelMenu)
        , shopMenu(shopMenu)
        , totalGoldRef(goldRef)
        , isMenuActive(true)
        , resetGoldRequested(false)
        , menuResult(0)
        , previousSelectedIndex(-1)
        , normalColor(Color::White)
        , selectedColor(Color::Yellow)
        , titleColor(Color::Red)
        , backgroundColor(Color(30, 30, 60, 200))
        , totalGoldColor(Color::Yellow)
        , buttonColor(Color(70, 70, 70, 180))
        , buttonOutlineColor(Color::White)
        , confirmationActive(false)
        , yesSelected(true)
    {
        initializeResources();
        initializeConfirmationDialog();
    }
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

    void resetShopUpgrades() {
        shopMenu.resetUpgrades();
    }

    bool isResetGoldRequested() const { return resetGoldRequested; }
    void clearResetGoldRequest() { resetGoldRequested = false; }
    void resetGold() { resetGoldRequested = true; }

    bool isLevelSelectActive() const { return levelMenu.isActive(); }
    LevelMenu& getLevelMenu() { return levelMenu; }
    bool isSettingsActive() const { return settingsMenu.isActive(); }
    SettingsMenu& getSettingsMenu() { return settingsMenu; }

    bool isShopActive() const { return shopMenu.isActiveState(); }
    ShopMenu& getShopMenu() { return shopMenu; }

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
    LevelMenu& levelMenu;
    ShopMenu shopMenu;

    Color normalColor;
    Color selectedColor;
    Color titleColor;
    Color backgroundColor;
    Color totalGoldColor;

    bool isMenuActive;
    bool resetGoldRequested;
    int menuResult;
    int& totalGoldRef;

    int previousSelectedIndex = -1;

    vector<RectangleShape> buttons; 
    Color buttonColor;
    Color buttonOutlineColor;

    void initializeResources();
    void initializeMenuItems();
    void initializeButtons();
    void updateMenuVisuals();
    void handleMenuSelection(int selectedIndex);

    bool confirmationActive;
    bool waitingForNewGameConfirmation;
    Text confirmationText;
    RectangleShape confirmationBox;
    Text yesText, noText;
    bool yesSelected;

    void initializeConfirmationDialog();
    void handleConfirmationEvents(Event& event);
    void renderConfirmationDialog();
    void updateConfirmationVisuals();
};

namespace MenuItems {
    const int START_GAME = 0;
    const int NEW_GAME = 1;
    const int SETTINGS = 2;
    const int SHOP = 3;
    const int EXIT = 4;
}