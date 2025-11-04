#pragma once
#include <SFML/Graphics.hpp>
#include "MenuController.h"
#include "AudioManager.h"
#include <vector>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class LevelMenu {
public:
    LevelMenu(RenderWindow& gameWindow, AudioManager& audioManager)
        : gameWindow(gameWindow)
        , audioManager(audioManager)
        , menuController(6)
        , isActiveState(false)
        , selectedLevel(0)
        , previousSelectedIndex(-1)
        , normalColor(Color::White)
        , selectedColor(Color::Yellow)
        , titleColor(Color::Cyan)
        , backgroundColor(Color(30, 60, 30, 200))
        , buttonColor(Color(70, 70, 70, 180))
        , buttonOutlineColor(Color::White)
    {
        initializeResources();
    }

    bool isActive() const { return isActiveState; }
    void setActive(bool active) {
        isActiveState = active;
        if (active) {
            menuController.resetSelection();
            updateMenuVisuals();
        }
    }

    int getSelectedLevel() const { return selectedLevel; }
    void resetSelection() { selectedLevel = 0; }

    void update();
    void render();

private:
    RenderWindow& gameWindow;
    AudioManager& audioManager;
    Font font;
    vector<Text> menuItems;
    vector<RectangleShape> buttons;
    Texture backgroundTexture;
    Sprite background;

    MenuController menuController;
    bool isActiveState;
    int selectedLevel;

    Color normalColor;
    Color selectedColor;
    Color titleColor;
    Color backgroundColor;
    Color buttonColor;
    Color buttonOutlineColor;

    int previousSelectedIndex;

    void initializeResources();
    void initializeMenuItems();
    void initializeButtons();
    void updateMenuVisuals();
    void handleMenuSelection(int selectedIndex);
    void handleEvents();
};

namespace LevelMenuItems {
    const int LEVEL_1 = 0;
    const int LEVEL_2 = 1;
    const int LEVEL_3 = 2;
    const int LEVEL_4 = 3;
    const int LEVEL_5 = 4;
    const int BACK = 5;
}