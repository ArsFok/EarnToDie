#include "GameMenu.h"
#include "GameState.h"
#include <iostream>
#include "const.h"

using namespace sf;
using namespace std;

void GameMenu::initializeResources() {
    if (!backgroundTexture.loadFromFile("menu_background.png")) {
        backgroundTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    background.setTexture(backgroundTexture);

    if (!font.loadFromFile("arial.ttf")) {
        std::vector<std::string> fallbackFonts = {
            "C:/Windows/Fonts/arial.ttf",
            "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
            "/System/Library/Fonts/Arial.ttf"
        };
        bool fontLoaded = false;
        for (const auto& fontPath : fallbackFonts) {
            if (font.loadFromFile(fontPath)) {
                fontLoaded = true;
                break;
            }
        }
        if (!fontLoaded) {
            throw std::runtime_error("Critical: Cannot load any font for menu!");
        }
    }
    initializeMenuItems();
    initializeButtons();
}

void GameMenu::initializeMenuItems() {
    vector<string> menuTexts = {
        "Start Game",
        "New Game",
        "Settings",
        "Shop",
        "Exit"
    };

    menuController.setMenuItemsCount(menuTexts.size());

    float startY = 250;

    for (size_t i = 0; i < menuTexts.size(); ++i) {
        Text text;
        text.setFont(font);
        text.setString(menuTexts[i]);
        text.setCharacterSize(30);
        text.setFillColor(normalColor);

        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        text.setPosition(WINDOW_WIDTH / 2.0f, startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        menuItems.push_back(text);
    }
}

void GameMenu::initializeButtons() {
    float startY = 250;

    for (size_t i = 0; i < menuItems.size(); ++i) {
        RectangleShape button(Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
        button.setFillColor(buttonColor);
        button.setOutlineThickness(2.0f);
        button.setOutlineColor(buttonOutlineColor);

        button.setOrigin(BUTTON_WIDTH / 2.0f, BUTTON_HEIGHT / 2.0f);
        button.setPosition(WINDOW_WIDTH / 2.0f, startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        buttons.push_back(button);
    }
}

void GameMenu::update() {
    if (confirmationActive) {
        Event event;
        while (gameWindow.pollEvent(event)) {
            handleConfirmationEvents(event);
        }
        return;
    }
    if (settingsMenu.isActive()) {
        settingsMenu.update();
        return;
    }
    if (shopMenu.isActiveState()) {
        shopMenu.update();
        return;
    }
    handleEvents();
    updateMenuVisuals();
}

void GameMenu::handleEvents() {
    Event event;
    while (gameWindow.pollEvent(event)) {
        switch (event.type) {
        case Event::Closed:
            gameWindow.close();
            break;

        case Event::KeyPressed:
            switch (event.key.code) {
            case Keyboard::Up:
            {
                int before = menuController.getSelectedIndex();
                menuController.moveUp();
                int after = menuController.getSelectedIndex();

                if (before != after) {
                    std::cout << "UP: " << before << " -> " << after << std::endl;
                }
                updateMenuVisuals();
            }
            break;
            case Keyboard::Down:
            {
                int before = menuController.getSelectedIndex();
                menuController.moveDown();
                int after = menuController.getSelectedIndex();

                if (before != after) {
                    std::cout << "DOWN: " << before << " -> " << after << std::endl;
                }
                updateMenuVisuals();
            }
            break;
            case Keyboard::Return:
            case Keyboard::Space:
            {
                int selectedIndex = menuController.getSelectedIndex();
                if (menuController.isValidIndex()) {
                    handleMenuSelection(selectedIndex);
                }
            }
            break;
            case Keyboard::Escape:
                if (shopMenu.isActiveState()) {
                    shopMenu.setActive(false);
                    cout << "Shop closed by ESC" << endl;
                }
                else if (settingsMenu.isActive()) {
                    settingsMenu.setActive(false);
                }
                else {
                    gameWindow.close();
                }
                break;
            default:
                break;
            }
            break;

        case Event::MouseMoved:
            for (size_t i = 0; i < buttons.size(); ++i) {
                FloatRect bounds = buttons[i].getGlobalBounds();
                if (bounds.contains(static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y))) {
                    menuController.setSelectedIndex(i);
                    updateMenuVisuals();
                }
            }
            break;

        case Event::MouseButtonPressed:
            if (event.mouseButton.button == Mouse::Left) {
                for (size_t i = 0; i < buttons.size(); ++i) {
                    FloatRect bounds = buttons[i].getGlobalBounds();
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

void GameMenu::render() {
    if (confirmationActive) {
        gameWindow.clear();
        RectangleShape backgroundOverlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        backgroundOverlay.setFillColor(Color(0, 0, 0, 150));
        gameWindow.draw(background);
        gameWindow.draw(backgroundOverlay);

        for (const auto& button : buttons) {
            RectangleShape transparentButton = button;
            transparentButton.setFillColor(Color(70, 70, 70, 100));
            gameWindow.draw(transparentButton);
        }
        for (const auto& item : menuItems) {
            Text transparentText = item;
            transparentText.setFillColor(Color(255, 255, 255, 100));
            gameWindow.draw(transparentText);
        }

        renderConfirmationDialog();
        gameWindow.display();
        return;
    }
    if (settingsMenu.isActive()) {
        settingsMenu.render();
        gameWindow.display();
        return;
    }
    if (shopMenu.isActiveState()) {
        shopMenu.render();
        gameWindow.display();
        return;
    }
    gameWindow.clear(backgroundColor);

    RectangleShape backgroundOverlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    backgroundOverlay.setFillColor(Color(0, 0, 0, 150));
    gameWindow.draw(background);
    gameWindow.draw(backgroundOverlay);

    Text title;
    title.setFont(font);
    title.setString("EARN TO DIE");
    title.setCharacterSize(53);
    title.setFillColor(titleColor);
    title.setStyle(sf::Text::Bold);

    FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    title.setPosition(WINDOW_WIDTH / 2.0f, 100);

    gameWindow.draw(title);

    Text subtitle;
    subtitle.setFont(font);
    subtitle.setString("Autor: MrArs_Fok");
    subtitle.setCharacterSize(18);
    subtitle.setFillColor(Color(200, 200, 200));

    FloatRect subtitleRect = subtitle.getLocalBounds();
    subtitle.setOrigin(subtitleRect.left + subtitleRect.width / 2.0f,
        subtitleRect.top + subtitleRect.height / 2.0f);
    subtitle.setPosition(WINDOW_WIDTH / 2.0f, 150);

    gameWindow.draw(subtitle);

    for (const auto& button : buttons) {
        gameWindow.draw(button);
    }

    for (const auto& item : menuItems) {
        gameWindow.draw(item);
    }

    Text controlsHint;
    controlsHint.setFont(font);
    controlsHint.setString("Use ARROW KEYS to navigate, ENTER to select");
    controlsHint.setCharacterSize(14);
    controlsHint.setFillColor(Color(200, 200, 200));
    controlsHint.setPosition(20, WINDOW_HEIGHT - 35);

    gameWindow.draw(controlsHint);

    Text totalGolds;
    totalGolds.setFont(font);
    totalGolds.setString("TotalGold:" + std::to_string(totalGoldRef));
    totalGolds.setCharacterSize(18);
    totalGolds.setFillColor(totalGoldColor);
    totalGolds.setPosition(10, 10);

    gameWindow.draw(totalGolds);

    Text selectionHint;
    selectionHint.setFont(font);
    selectionHint.setString("Selected: " + menuItems[menuController.getSelectedIndex()].getString());
    selectionHint.setCharacterSize(12);
    selectionHint.setFillColor(Color(100, 200, 100));
    selectionHint.setPosition(20, WINDOW_HEIGHT - 55);

    gameWindow.draw(selectionHint);

    gameWindow.display();
}

void GameMenu::updateMenuVisuals() {
    int selectedIndex = menuController.getSelectedIndex();
    if (selectedIndex != previousSelectedIndex) {
        std::cout << "=== SELECTION CHANGED ===" << std::endl;
        std::cout << "Previous: " << previousSelectedIndex << " -> New: " << selectedIndex << std::endl;
        std::cout << "Selected item: " << menuItems[selectedIndex].getString().toAnsiString() << std::endl;
        previousSelectedIndex = selectedIndex;
    }

    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (i >= buttons.size()) continue;

        if (i == selectedIndex) {
            buttons[i].setFillColor(Color(100, 100, 100, 200));
            buttons[i].setOutlineColor(selectedColor);
            buttons[i].setOutlineThickness(3.0f);

            menuItems[i].setFillColor(selectedColor);
            menuItems[i].setStyle(sf::Text::Bold);
            menuItems[i].setScale(1.03f, 1.03f);
        }
        else {
            buttons[i].setFillColor(buttonColor);
            buttons[i].setOutlineColor(buttonOutlineColor);
            buttons[i].setOutlineThickness(2.0f);

            menuItems[i].setFillColor(normalColor);
            menuItems[i].setStyle(sf::Text::Regular);
            menuItems[i].setScale(1.0f, 1.0f);
        }
    }
}

void GameMenu::handleMenuSelection(int selectedIndex) {
    std::cout << "=== MENU SELECTION ===" << std::endl;
    std::cout << "Selected index: " << selectedIndex << std::endl;
    std::cout << "Menu item: " << menuItems[selectedIndex].getString().toAnsiString() << std::endl;
    cout << "BEFORE - GameMenu: " << isMenuActive << ", LevelMenu: " << levelMenu.isActive() << endl;

    if (audioManager.isSoundLoaded("click")) {
        audioManager.playSound("click");
    }
    switch (selectedIndex) {
    case MenuItems::START_GAME:
        std::cout << "ACTION: Opening level selection..." << std::endl;
        levelMenu.setActive(true);
        isMenuActive = false;
        break;
    case MenuItems::NEW_GAME:
        std::cout << "ACTION: New game confirmation requested..." << std::endl;
        std::cout << "Activating confirmation dialog..." << std::endl;
        confirmationActive = true;
        waitingForNewGameConfirmation = true;
        yesSelected = true;
        updateConfirmationVisuals();
        break;
    case MenuItems::SETTINGS:
        std::cout << "Settings selected" << std::endl;
        settingsMenu.setActive(true);
        break;

    case MenuItems::SHOP:
        std::cout << "Shop selected" << std::endl;
        shopMenu.setActive(true);
        break;

    case MenuItems::EXIT:
        std::cout << "ACTION: Exit selected" << std::endl;
        gameWindow.close();
        break;

    default:
        break;
    }
    cout << "AFTER - GameMenu: " << isMenuActive << ", LevelMenu: " << levelMenu.isActive() << endl;
}

void GameMenu::resetMenu() {
    isMenuActive = true;
    menuController.resetSelection();
    levelMenu.resetSelection();
    updateMenuVisuals();
}
void GameMenu::initializeConfirmationDialog() {
    confirmationBox.setSize(Vector2f(500, 250));
    confirmationBox.setFillColor(Color(50, 50, 80, 240));
    confirmationBox.setOutlineThickness(3);
    confirmationBox.setOutlineColor(Color::White);
    confirmationBox.setOrigin(250, 125);
    confirmationBox.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);


    confirmationText.setFont(font);
    confirmationText.setString("Are you sure?\nThis will reset all gold and upgrades!");
    confirmationText.setCharacterSize(22);
    confirmationText.setFillColor(Color::White);
    confirmationText.setStyle(Text::Bold);
    confirmationText.setLineSpacing(1.2f);

    FloatRect textRect = confirmationText.getLocalBounds();
    confirmationText.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    confirmationText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 40);

    yesText.setFont(font);
    yesText.setString("YES");
    yesText.setCharacterSize(24);
    yesText.setFillColor(Color::Yellow);

    FloatRect yesRect = yesText.getLocalBounds();
    yesText.setOrigin(yesRect.left + yesRect.width / 2.0f,
        yesRect.top + yesRect.height / 2.0f);
    yesText.setPosition(WINDOW_WIDTH / 2 - 90, WINDOW_HEIGHT / 2 + 50);
 
    noText.setFont(font);
    noText.setString("NO");
    noText.setCharacterSize(24);
    noText.setFillColor(Color::White);

    FloatRect noRect = noText.getLocalBounds();
    noText.setOrigin(noRect.left + noRect.width / 2.0f,
        noRect.top + noRect.height / 2.0f);
    noText.setPosition(WINDOW_WIDTH / 2 + 90, WINDOW_HEIGHT / 2 + 50);

    confirmationActive = false;
    waitingForNewGameConfirmation = false;
    yesSelected = true;
}
void GameMenu::handleConfirmationEvents(Event& event) {
    if (!confirmationActive) return;

    switch (event.type) {
    case Event::KeyPressed:
        switch (event.key.code) {
        case Keyboard::Left:
        case Keyboard::Right:
            yesSelected = !yesSelected;
            updateConfirmationVisuals();
            break;
        case Keyboard::Enter:
        case Keyboard::Space:
            if (yesSelected) {
                if (waitingForNewGameConfirmation) {

                    resetGoldRequested = true;

                    levelMenu.setActive(true);
                    isMenuActive = false;

                    std::cout << "DEBUG: New game confirmed - opening level selection" << std::endl;
                }
            }
            confirmationActive = false;
            waitingForNewGameConfirmation = false;
            break;
        case Keyboard::Escape:
            confirmationActive = false;
            waitingForNewGameConfirmation = false;
            std::cout << "New game cancelled" << std::endl;
            break;
        default:
            break;
        }
        break;

    case Event::MouseMoved:
    {
        FloatRect yesBounds = yesText.getGlobalBounds();
        FloatRect noBounds = noText.getGlobalBounds();

        if (yesBounds.contains(static_cast<float>(event.mouseMove.x),
            static_cast<float>(event.mouseMove.y))) {
            yesSelected = true;
            updateConfirmationVisuals();
        }
        else if (noBounds.contains(static_cast<float>(event.mouseMove.x),
            static_cast<float>(event.mouseMove.y))) {
            yesSelected = false;
            updateConfirmationVisuals();
        }
    }
    break;

    case Event::MouseButtonPressed:
        if (event.mouseButton.button == Mouse::Left) {
            FloatRect yesBounds = yesText.getGlobalBounds();
            FloatRect noBounds = noText.getGlobalBounds();

            if (yesBounds.contains(static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y))) {
                if (waitingForNewGameConfirmation) {
                    resetGoldRequested = true;

                    levelMenu.setActive(true);
                    isMenuActive = false;

                    std::cout << "New game confirmed!" << std::endl;
                }
                confirmationActive = false;
                waitingForNewGameConfirmation = false;
            }
            else if (noBounds.contains(static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y))) {
                confirmationActive = false;
                waitingForNewGameConfirmation = false;
                std::cout << "New game cancelled" << std::endl;
            }
        }
        break;

    default:
        break;
    }
}

void GameMenu::updateConfirmationVisuals() {
    if (yesSelected) {
        yesText.setFillColor(Color::Yellow);
        yesText.setStyle(Text::Bold);
        noText.setFillColor(Color::White);
        noText.setStyle(Text::Regular);
    }
    else {
        yesText.setFillColor(Color::White);
        yesText.setStyle(Text::Regular);
        noText.setFillColor(Color::Yellow);
        noText.setStyle(Text::Bold);
    }
}

void GameMenu::renderConfirmationDialog() {
    if (!confirmationActive) return;
    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(0, 0, 0, 150));
    gameWindow.draw(overlay);

    gameWindow.draw(confirmationBox);
    gameWindow.draw(confirmationText);
    gameWindow.draw(yesText);
    gameWindow.draw(noText);
}