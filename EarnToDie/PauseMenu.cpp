#include "PauseMenu.h"
#include <iostream>
#include "const.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, AudioManager& audioManager)
    : gameWindow(window)
    , menuController(5)
    , settingsMenu(window, audioManager)
    , normalColor(sf::Color::White)
    , selectedColor(sf::Color::Yellow)
    , buttonColor(sf::Color(70, 70, 70, 200))
    , buttonOutlineColor(sf::Color::White)
    , previousSelectedIndex(-1) {

    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Failed to load font for pause menu!" << std::endl;
    }

    initializeMenuItems();
    initializeButtons();
    setActive(false);
    setGamePaused(false);
}

void PauseMenu::setActive(bool active) {
    isMenuActive = active;
    if (active) {
        menuController.resetSelection();
        updateMenuVisuals();
    }
}

void PauseMenu::initializeMenuItems() {
    std::vector<std::string> menuTexts = {
        "Continue",
        "Settings",
        "Shop",
        "Main Menu",
        "Exit Game"
    };

    menuController.setMenuItemsCount(menuTexts.size());

    float startY = WINDOW_HEIGHT / 2 - (menuTexts.size() * (BUTTON_HEIGHT + BUTTON_PADDING)) / 2;

    for (size_t i = 0; i < menuTexts.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(menuTexts[i]);
        text.setCharacterSize(30);
        text.setFillColor(normalColor);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        text.setPosition(WINDOW_WIDTH / 2.0f, startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        menuPauseItems.push_back(text);
    }
}

void PauseMenu::initializeButtons() {
    float startY = WINDOW_HEIGHT / 2 - (menuPauseItems.size() * (BUTTON_HEIGHT + BUTTON_PADDING)) / 2;

    for (size_t i = 0; i < menuPauseItems.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
        button.setFillColor(buttonColor);
        button.setOutlineThickness(2.0f);
        button.setOutlineColor(buttonOutlineColor);

        button.setOrigin(BUTTON_WIDTH / 2.0f, BUTTON_HEIGHT / 2.0f);
        button.setPosition(WINDOW_WIDTH / 2.0f, startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        buttons.push_back(button);
    }
}

void PauseMenu::update() {
    handleEvents();

    if (settingsMenu.isActive()) {
        settingsMenu.update(); 
    }
    else {
        updateMenuVisuals(); // Это для паузы
    }
}

void PauseMenu::handleEvents() {
    if (settingsMenu.isActive()) {
        settingsMenu.handleEvents();
        return;
    }
    sf::Event event;
    while (gameWindow.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            gameWindow.close();
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code) {
            case sf::Keyboard::Up:
            {
                int before = menuController.getSelectedIndex();
                menuController.moveUp();
                int after = menuController.getSelectedIndex();

                if (before != after) {
                    std::cout << "=== PAUSE MENU ===" << std::endl;
                    std::cout << "UP: " << before << " -> " << after << std::endl;
                }
            }
            break;
            case sf::Keyboard::Down:
            {
                int before = menuController.getSelectedIndex();
                menuController.moveDown();
                int after = menuController.getSelectedIndex();

                if (before != after) {
                    std::cout << "=== PAUSE MENU ===" << std::endl;
                    std::cout << "DOWN: " << before << " -> " << after << std::endl;
                }
            }
            break;
            case sf::Keyboard::Return:
            case sf::Keyboard::Space:
            {
                int selectedIndex = menuController.getSelectedIndex();
                if (menuController.isValidIndex()) {
                    std::cout << "=== PAUSE MENU ACTION ===" << std::endl;
                    std::cout << "ENTER/SPACE pressed on: " << selectedIndex << std::endl;
                    handleMenuSelection(selectedIndex);
                }
            }
            break;
            case sf::Keyboard::Escape:
                std::cout << "=== PAUSE MENU ===" << std::endl;
                std::cout << "ESC pressed - resuming game" << std::endl;
                setGamePaused(false);
                setActive(false);
                break;
            default:
                break;
            }
            break;

        case sf::Event::MouseMoved:
            for (size_t i = 0; i < buttons.size(); ++i) {
                sf::FloatRect bounds = buttons[i].getGlobalBounds();
                if (bounds.contains(static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y))) {
                    menuController.setSelectedIndex(i);
                }
            }
            break;

        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < buttons.size(); ++i) {
                    sf::FloatRect bounds = buttons[i].getGlobalBounds();
                    if (bounds.contains(static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y))) {
                        handleMenuSelection(i);
                    }
                }
            }
            break;

        default:
            break;
        }
    }
}

void PauseMenu::updateMenuVisuals() {
    int selectedIndex = menuController.getSelectedIndex();

    if (!menuController.isValidIndex()) {
        return;
    }

    if (selectedIndex != previousSelectedIndex) {
        std::cout << "=== PAUSE MENU SELECTION ===" << std::endl;
        std::cout << "Changed: " << previousSelectedIndex << " -> " << selectedIndex << std::endl;
        std::cout << "Now selected: " << menuPauseItems[selectedIndex].getString().toAnsiString() << std::endl;
        previousSelectedIndex = selectedIndex;
    }

    for (size_t i = 0; i < menuPauseItems.size(); ++i) {
        if (i >= buttons.size()) continue;

        if (i == selectedIndex) {
            buttons[i].setFillColor(sf::Color(100, 100, 100, 200));
            buttons[i].setOutlineColor(selectedColor);
            buttons[i].setOutlineThickness(3.0f);
            menuPauseItems[i].setFillColor(selectedColor);
            menuPauseItems[i].setStyle(sf::Text::Bold);
            menuPauseItems[i].setScale(1.05f, 1.05f);
        }
        else {
            buttons[i].setFillColor(buttonColor);
            buttons[i].setOutlineColor(buttonOutlineColor);
            buttons[i].setOutlineThickness(2.0f);
            menuPauseItems[i].setFillColor(normalColor);
            menuPauseItems[i].setStyle(sf::Text::Regular);
            menuPauseItems[i].setScale(1.0f, 1.0f);
        }
    }
}

void PauseMenu::handleMenuSelection(int selectedIndex) {
    if (!menuController.isValidIndex()) return;

    std::cout << "=== PAUSE MENU FINAL SELECTION ===" << std::endl;
    std::cout << "Selected: " << selectedIndex << " - " << menuPauseItems[selectedIndex].getString().toAnsiString() << std::endl;

    pauseMenuResult = selectedIndex;

    switch (selectedIndex) {
    case 0: // RESUME
        std::cout << "ACTION: Resuming game..." << std::endl;
        setGamePaused(false);
        setActive(false);
        break;
    case 1: // SETTINGS
        std::cout << "Settings selected from pause menu" << std::endl;
        settingsMenu.setActive(true);
        break;
    case 2: // SHOP
        std::cout << "Shop selected from pause menu" << std::endl;
        break;
    case 3: // MAIN_MENU
        std::cout << "ACTION: Returning to main menu..." << std::endl;
        setGamePaused(false);
        setActive(false);
        break;
    case 4: // EXIT
        std::cout << "ACTION: Exiting game from pause menu" << std::endl;
        gameWindow.close();
        break;
    default:
        break;
    }
}

void PauseMenu::render() {
    if (settingsMenu.isActive()) {
        settingsMenu.render();
        return;
    }
    // Полупрозрачный темный фон
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    gameWindow.draw(overlay);

    // Заголовок паузы
    sf::Text pauseTitle;
    pauseTitle.setFont(font);
    pauseTitle.setString("GAME PAUSED");
    pauseTitle.setCharacterSize(60);
    pauseTitle.setFillColor(sf::Color::Yellow);
    pauseTitle.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = pauseTitle.getLocalBounds();
    pauseTitle.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    pauseTitle.setPosition(WINDOW_WIDTH / 2.0f, 150);
    gameWindow.draw(pauseTitle);

    // Рисуем кнопки
    for (const auto& button : buttons) {
        gameWindow.draw(button);
    }

    // Рисуем текст на кнопках
    for (const auto& text : menuPauseItems) {
        gameWindow.draw(text);
    }

    // Подсказки управления
    sf::Text controlsHint;
    controlsHint.setFont(font);
    controlsHint.setString("Use ARROW KEYS and ENTER to select, ESC to continue");
    controlsHint.setCharacterSize(18);
    controlsHint.setFillColor(sf::Color(200, 200, 200));

    sf::FloatRect hintRect = controlsHint.getLocalBounds();
    controlsHint.setOrigin(hintRect.left + hintRect.width / 2.0f,
        hintRect.top + hintRect.height / 2.0f);
    controlsHint.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 50);
    gameWindow.draw(controlsHint);
}