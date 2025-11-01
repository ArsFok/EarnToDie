#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class FinalGameWindow {
private:
    static constexpr float BUTTON_WIDTH = 300.0f;
    static constexpr float BUTTON_HEIGHT = 45.0f;
    static constexpr float BUTTON_PADDING = 15.0f;

    sf::RenderWindow& gameWindow;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite background;

    std::vector<sf::Text> menuItems;
    std::vector<sf::RectangleShape> buttons;

    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color buttonColor;
    sf::Color buttonOutlineColor;
    sf::Color titleColor;

    bool isActive;
    int selectedIndex;
    int finalResult;

    void initializeMenuItems();
    void initializeButtons();

public:
    FinalGameWindow(sf::RenderWindow& window);
    ~FinalGameWindow() = default;

    sf::RenderWindow& getWindow() { return gameWindow; }
    bool isActiveState() const { return isActive; }
    void setActive(bool active, int result = 0);
    int getSelectedAction() const { return selectedIndex; }
    void resetSelection() { selectedIndex = -1; }

    void update();
    void render();
    void handleEvents();

    enum FinalActions {
        RESTART_GAME = 0,
        MAIN_MENU = 1,
        EXIT_GAME = 2
    };
};