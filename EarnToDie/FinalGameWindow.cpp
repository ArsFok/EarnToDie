#include "FinalGameWindow.h"
#include "const.h"
#include <iostream>

using namespace sf;

FinalGameWindow::FinalGameWindow(RenderWindow& window)
    : gameWindow(window)
    , normalColor(Color::White)
    , selectedColor(Color::Yellow)
    , buttonColor(Color(70, 70, 70, 200))
    , buttonOutlineColor(Color::White)
    , titleColor(Color::Red)
    , isActive(false)
    , selectedIndex(-1)
    , finalResult(0) {

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
            throw std::runtime_error("Critical: Cannot load any font for final window!");
        }
    }

    initializeMenuItems();
    initializeButtons();
}

void FinalGameWindow::initializeMenuItems() {
    std::vector<std::string> menuTexts = {
        "Restart Game",
        "Main Menu",
        "Exit Game"
    };

    float startY = WINDOW_HEIGHT / 2 - (menuTexts.size() * (BUTTON_HEIGHT + BUTTON_PADDING)) / 2 + 50;

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

void FinalGameWindow::initializeButtons() {
    float startY = WINDOW_HEIGHT / 2 - (menuItems.size() * (BUTTON_HEIGHT + BUTTON_PADDING)) / 2 + 50;

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

void FinalGameWindow::setActive(bool active, int result) {
    isActive = active;
    if (active) {
        finalResult = result;
        selectedIndex = -1;
        std::cout << "Final game window activated with result: " << result << std::endl;
    }
}

void FinalGameWindow::update() {
    handleEvents();

    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (i == selectedIndex) {
            buttons[i].setFillColor(Color(100, 100, 100, 200));
            buttons[i].setOutlineColor(selectedColor);
            buttons[i].setOutlineThickness(3.0f);
            menuItems[i].setFillColor(selectedColor);
            menuItems[i].setStyle(Text::Bold);
            menuItems[i].setScale(1.03f, 1.03f);
        }
        else {
            buttons[i].setFillColor(buttonColor);
            buttons[i].setOutlineColor(buttonOutlineColor);
            buttons[i].setOutlineThickness(2.0f);
            menuItems[i].setFillColor(normalColor);
            menuItems[i].setStyle(Text::Regular);
            menuItems[i].setScale(1.0f, 1.0f);
        }
    }
}

void FinalGameWindow::handleEvents() {
    if (!isActive) return;

    Event event;
    while (gameWindow.pollEvent(event)) {
        switch (event.type) {
        case Event::Closed:
            gameWindow.close();
            break;

        case Event::KeyPressed:
            switch (event.key.code) {
            case Keyboard::Up:
                if (selectedIndex > 0) {
                    selectedIndex--;
                    std::cout << "Final window: UP to " << selectedIndex << std::endl;
                }
                break;
            case Keyboard::Down:
                if (selectedIndex < static_cast<int>(menuItems.size()) - 1) {
                    selectedIndex++;
                    std::cout << "Final window: DOWN to " << selectedIndex << std::endl;
                }
                break;
            case Keyboard::Return:
            case Keyboard::Space:
                if (selectedIndex >= 0) {
                    std::cout << "Final window: ACTION " << selectedIndex << std::endl;
                    isActive = false;
                }
                break;
            case Keyboard::Escape:
                std::cout << "Final window: ESC pressed" << std::endl;
                selectedIndex = FinalActions::MAIN_MENU;
                isActive = false;
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
                    selectedIndex = i;
                    break;
                }
            }
            break;

        case Event::MouseButtonPressed:
            if (event.mouseButton.button == Mouse::Left) {
                for (size_t i = 0; i < buttons.size(); ++i) {
                    FloatRect bounds = buttons[i].getGlobalBounds();
                    if (bounds.contains(static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y))) {
                        selectedIndex = i;
                        isActive = false;
                        std::cout << "Final window: CLICK on " << i << std::endl;
                        break;
                    }
                }
            }
            break;

        default:
            break;
        }
    }
}

void FinalGameWindow::render() {
    if (!isActive) return;

    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(0, 0, 0, 180));
    gameWindow.draw(background);
    gameWindow.draw(overlay);

    Text title;
    title.setFont(font);

    if (finalResult == 1) {
        title.setString("VICTORY!");
        title.setFillColor(Color::Green);
    }
    else {
        title.setString("GAME OVER");
        title.setFillColor(Color::Red);
    }

    title.setCharacterSize(70);
    title.setStyle(Text::Bold);

    FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    title.setPosition(WINDOW_WIDTH / 2.0f, 150);

    gameWindow.draw(title);
    Text message;
    message.setFont(font);

    if (finalResult == 1) {
        message.setString("Congratulations! You won the game!");
    }
    else {
        message.setString("Better luck next time!");
    }

    message.setCharacterSize(28);
    message.setFillColor(Color::White);

    FloatRect messageRect = message.getLocalBounds();
    message.setOrigin(messageRect.left + messageRect.width / 2.0f,
        messageRect.top + messageRect.height / 2.0f);
    message.setPosition(WINDOW_WIDTH / 2.0f, 220);

    gameWindow.draw(message);

    for (const auto& button : buttons) {
        gameWindow.draw(button);
    }

    for (const auto& text : menuItems) {
        gameWindow.draw(text);
    }
    Text controlsHint;
    controlsHint.setFont(font);
    controlsHint.setString("Use ARROW KEYS and ENTER to select, ESC for Main Menu");
    controlsHint.setCharacterSize(16);
    controlsHint.setFillColor(Color(200, 200, 200));

    FloatRect hintRect = controlsHint.getLocalBounds();
    controlsHint.setOrigin(hintRect.left + hintRect.width / 2.0f,
        hintRect.top + hintRect.height / 2.0f);
    controlsHint.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 50);

    gameWindow.draw(controlsHint);
}