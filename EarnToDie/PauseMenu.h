#pragma once
#include "EntityController.h"
#include "const.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PauseMenu{
public:
	PauseMenu(RenderWindow& gameWindow, EntityController& controller);
	~PauseMenu() = default;

	void update();
	void render();
	void handleEvents();

	bool isActive() const { return entityController.isGamePaused(); }
	int getSelectedAction() const { return entityController.getSelectedMenuIndex(); }
	int getPauseMenuResult() { return pauseMenuResult; }

private:
	RenderWindow& gameWindow;
	Font font;
	std::vector<Text> menuPauseItems;
	std::vector<RectangleShape> buttons;

	EntityController& entityController;

	int pauseMenuResult;
	int previousSelectedIndex = -1;

	Color normalColor;
	Color selectedColor;
	Color titleColor;
	Color buttonColor;
	Color buttonOutlineColor;

	void initializeMenuItems();
	void initializeButtons();
	void updateMenuVisuals();
	void handleMenuSelection(int selectedIndex);
};
namespace PauseMenuItems {
	const int RESUME = 0;
	const int SETTINGS = 1;
	const int SHOP = 2;
	const int MAIN_MENU = 3;
	const int EXIT = 4;
}
