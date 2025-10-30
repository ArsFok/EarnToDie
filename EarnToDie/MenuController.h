#pragma once
class MenuController {
public:
    MenuController(int menuItemsCount);

    // Навигация по меню
    void moveUp();
    void moveDown();
    void resetSelection();
    int getSelectedIndex() const { return selectedIndex; }

    // Установка количества пунктов меню
    void setMenuItemsCount(int count) {
        menuItemsCount = count;
        // Автоматически сбрасываем выделение при изменении количества пунктов
        if (selectedIndex >= menuItemsCount) {
            selectedIndex = 0;
        }
    }
    int getMenuItemsCount() const { return menuItemsCount; }

    // Дополнительные методы для удобства
    bool isValidIndex() const { return selectedIndex >= 0 && selectedIndex < menuItemsCount; }
    void setSelectedIndex(int index) {
        if (index >= 0 && index < menuItemsCount) {
            selectedIndex = index;
        }
    }

private:
    int selectedIndex = 0;
    int menuItemsCount;
};