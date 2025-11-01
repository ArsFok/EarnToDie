#include "MenuController.h"

MenuController::MenuController(int menuItemsCount)
    : selectedIndex(0), menuItemsCount(menuItemsCount) {
}

void MenuController::moveUp() {
    selectedIndex--;
    if (selectedIndex < 0) {
        selectedIndex = menuItemsCount - 1;
    }
}

void MenuController::moveDown() {
    selectedIndex++;
    if (selectedIndex >= menuItemsCount) {
        selectedIndex = 0;
    }
}

void MenuController::resetSelection() {
    selectedIndex = 0;
}