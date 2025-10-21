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

	bool isMenuActive;
	int menuResult;

	EntityController& entityController;
	
	void initializeMenuItems();
	void updateMenuVisuals();
	void handleMenuSelection(int selectedIndex);
};

namespace MenuItems {
	const int START_GAME = 0;
	const int SETTINGS = 1;
	const int SHOP = 2;
	const int EXIT = 3;
}

