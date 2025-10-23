#pragma once
#include <SFML/Graphics.hpp>
#include "EntityController.h"
#include <vector>
#include <string>

using namespace sf;
using namespace std;

class GameMenu{
public:
	GameMenu(RenderWindow& gameWindow, EntityController& controller);
	~GameMenu() = default;

	bool isActive() const { return isMenuActive; }
	void setActive(bool active) { isMenuActive = active; }
	int getMenuResult() const { return menuResult; }
	void resetMenu();

	bool isResetGoldRequested() const { return resetGoldRequested; }
	void clearResetGoldRequest() { resetGoldRequested = false; }
	void setTotalGold(int gold) { totalGoldValue = gold; }
	void resetGold(){ resetGoldRequested = true; }

	void update();
	void render();
	void handleEvents();

private:
	RenderWindow& gameWindow;
	Font font;
	vector<Text> menuItems;
	int selectedItemIndex;
	Texture backgroundTexture;
	Sprite background;

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

	EntityController& entityController;
	
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

