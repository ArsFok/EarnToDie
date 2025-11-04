#include "LevelMenu.h"
#include "const.h"
#include <vector>

using namespace sf;
using namespace std;

void LevelMenu::initializeResources() {
    if (!backgroundTexture.loadFromFile("level_select_background.png")) {
        cout << "Level select background not found, trying main menu background..." << endl;
        if (!backgroundTexture.loadFromFile("menu_background.png")) {
            cout << "Main menu background not found, creating default background..." << endl;
            backgroundTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

            // Заполняем текстуру градиентом
            Image backgroundImage;
            backgroundImage.create(WINDOW_WIDTH, WINDOW_HEIGHT, Color(30, 60, 90));
            backgroundTexture.loadFromImage(backgroundImage);
        }
    }
    background.setTexture(backgroundTexture);

    // Загрузка шрифта
    if (!font.loadFromFile("arial.ttf")) {
        vector<string> fallbackFonts = {
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
            throw runtime_error("Critical: Cannot load any font for level menu!");
        }
    }

    initializeMenuItems();
    initializeButtons();
}

void LevelMenu::initializeMenuItems() {
    vector<string> menuTexts = {
        "Level 1",
        "Level 2",
        "Level 3",
        "Level 4",
        "Level 5",
        "Back to Main Menu"
    };

    // Устанавливаем количество пунктов меню в контроллере
    menuController.setMenuItemsCount(menuTexts.size());

    float startY = 200;

    for (size_t i = 0; i < menuTexts.size(); ++i) {
        Text text;
        text.setFont(font);
        text.setString(menuTexts[i]);
        text.setCharacterSize(28);
        text.setFillColor(normalColor);

        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        text.setPosition(WINDOW_WIDTH / 2.0f, startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        menuItems.push_back(text);
    }
}

void LevelMenu::initializeButtons() {
    float startY = 200;

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

void LevelMenu::update() {
    handleEvents();
    updateMenuVisuals();
}

void LevelMenu::handleEvents() {
    Event event;
    while (gameWindow.pollEvent(event)) {
        switch (event.type) {
        case Event::Closed:
            gameWindow.close();
            break;

        case Event::KeyPressed:
            switch (event.key.code) {
            case Keyboard::Up:
                menuController.moveUp();
                updateMenuVisuals();
                break;
            case Keyboard::Down:
                menuController.moveDown();
                updateMenuVisuals();
                break;
            case Keyboard::Enter:
            case Keyboard::Space:
                if (menuController.isValidIndex()) {
                    handleMenuSelection(menuController.getSelectedIndex());
                }
                break;
            case Keyboard::Escape:
                isActiveState = false;
                selectedLevel = 0;
                if (audioManager.isSoundLoaded("click")) {
                    audioManager.playSound("click");
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

void LevelMenu::render() {

    gameWindow.clear(backgroundColor);

    // Фон
    RectangleShape backgroundOverlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    backgroundOverlay.setFillColor(Color(0, 0, 0, 150));
    gameWindow.draw(background);
    gameWindow.draw(backgroundOverlay);

    // Заголовок
    Text title;
    title.setFont(font);
    title.setString("SELECT LEVEL");
    title.setCharacterSize(48);
    title.setFillColor(titleColor);
    title.setStyle(Text::Bold);

    FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    title.setPosition(WINDOW_WIDTH / 2.0f, 80);

    gameWindow.draw(title);

    // Кнопки
    for (const auto& button : buttons) {
        gameWindow.draw(button);
    }

    // Текст на кнопках
    for (const auto& item : menuItems) {
        gameWindow.draw(item);
    }

    // Подсказки управления
    Text controlsHint;
    controlsHint.setFont(font);
    controlsHint.setString("Use ARROW KEYS to navigate, ENTER to select, ESC to go back");
    controlsHint.setCharacterSize(14);
    controlsHint.setFillColor(Color(200, 200, 200));
    controlsHint.setPosition(20, WINDOW_HEIGHT - 30);

    gameWindow.draw(controlsHint);

    gameWindow.display();
}

void LevelMenu::updateMenuVisuals() {
    if (!menuController.isValidIndex()) {
        return;
    }

    int selectedIndex = menuController.getSelectedIndex();

    if (selectedIndex != previousSelectedIndex) {
        cout << "Level Menu: " << previousSelectedIndex << " -> " << selectedIndex << endl;
        previousSelectedIndex = selectedIndex;
    }

    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (i >= buttons.size()) continue;

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

void LevelMenu::handleMenuSelection(int selectedIndex) {
    cout << "Level selected: " << selectedIndex << endl;

    switch (selectedIndex) {
    case LevelMenuItems::LEVEL_1:
        selectedLevel = 1;
        break;
    case LevelMenuItems::LEVEL_2:
        selectedLevel = 2;
        break;
    case LevelMenuItems::LEVEL_3:
        selectedLevel = 3;
        break;
    case LevelMenuItems::LEVEL_4:
        selectedLevel = 4;
        break;
    case LevelMenuItems::LEVEL_5:
        selectedLevel = 5;
        break;
    case LevelMenuItems::BACK:
        selectedLevel = 0;
        isActiveState = false;
        break;
    default:
        break;
    }

    if (audioManager.isSoundLoaded("click")) {
        audioManager.playSound("click");
    }
}