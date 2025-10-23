#include "GameMenu.h"
#include "GameState.h"
#include <iostream>
#include "const.h"

using namespace sf;

GameMenu::GameMenu(RenderWindow& window, EntityController& controller):
    gameWindow(window),
    entityController(controller),
	normalColor(Color::White),
	selectedColor(Color::Yellow),
	titleColor(Color::Red),
    totalGold(Color::Yellow),
    backgroundColor(sf::Color(30, 30, 60, 200)),
    isMenuActive(true),
    resetGoldRequested(false),
    menuResult(MenuItems::START_GAME),
    previousSelectedIndex(-1) {

	if (!backgroundTexture.loadFromFile("menu_background.jpg")) {
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
}

void GameMenu::initializeMenuItems() {
	vector<string> menuTexts = {
		"Start Game",
        "New Game",
		"Settings",
		"Shop",
		"Exit"
	};
	for (size_t i = 0; i < menuTexts.size(); ++i) {
		Text text;
		text.setFont(font);
		text.setString(menuTexts[i]);
		text.setCharacterSize(40);
		text.setFillColor(normalColor);

		FloatRect textRect = text.getGlobalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f,
					  textRect.top + textRect.height / 2.0f);
		text.setPosition(WINDOW_WIDTH / 2.0f, 250 + i * 70);

		menuItems.push_back(text);
	}
}

void GameMenu::update() {
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
                if (!entityController.isGamePaused() && !entityController.isGameFinal()) {
                    int before = entityController.getSelectedMenuIndex();
                    entityController.moveMainMenuUp();
                    int after = entityController.getSelectedMenuIndex();

                    // Выводим только если индекс действительно изменился
                    if (before != after) {
                        std::cout << "UP: " << before << " -> " << after << std::endl;
                    }
                    updateMenuVisuals();
                }
                break;
            case Keyboard::Down:
                if (!entityController.isGamePaused() && !entityController.isGameFinal()) {
                    int before = entityController.getSelectedMenuIndex();
                    entityController.moveMainMenuDown();
                    int after = entityController.getSelectedMenuIndex();

                    if (before != after) {
                        std::cout << "DOWN: " << before << " -> " << after << std::endl;
                    }
                    updateMenuVisuals();
                }
                break;
            case Keyboard::Return:
            case Keyboard::Space:
                handleMenuSelection(entityController.getSelectedMenuIndex());
                break;
            case Keyboard::Escape:
                gameWindow.close();
                break;
            default:
                break;
            }
            break;

        case Event::MouseMoved:
            // Обработка hover эффекта мышью
            for (size_t i = 0; i < menuItems.size(); ++i) {
                FloatRect bounds = menuItems[i].getGlobalBounds();
                if (bounds.contains(static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y))) {
                    if (!entityController.isGamePaused() && !entityController.isGameFinal()) {
                        entityController.resetMenuSelection();
                        for (int j = 0; j < i; j++) {
                            entityController.moveMainMenuDown();
                        }
                        updateMenuVisuals();
                    }
                }
            }
            break;

        case Event::MouseButtonPressed:
            if (event.mouseButton.button == Mouse::Left) {
                for (size_t i = 0; i < menuItems.size(); ++i) {
                    FloatRect bounds = menuItems[i].getGlobalBounds();
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
    gameWindow.clear(backgroundColor);

    RectangleShape backgroundOverlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    backgroundOverlay.setFillColor(Color(0, 0, 0, 180));
    gameWindow.draw(background);

    // Заголовок игры
    Text title;
    title.setFont(font);
    title.setString("EARN TO DIE");
    title.setCharacterSize(70);
    title.setFillColor(titleColor);
    title.setStyle(sf::Text::Bold);

    FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    title.setPosition(WINDOW_WIDTH / 2.0f, 120);

    gameWindow.draw(title);

    // Подзаголовок
    Text subtitle;
    subtitle.setFont(font);
    subtitle.setString("Autor: MrArs_Fok");
    subtitle.setCharacterSize(24);
    subtitle.setFillColor(Color(200, 200, 200));

    FloatRect subtitleRect = subtitle.getLocalBounds();
    subtitle.setOrigin(subtitleRect.left + subtitleRect.width / 2.0f,
        subtitleRect.top + subtitleRect.height / 2.0f);
    subtitle.setPosition(WINDOW_WIDTH / 2.0f, 180);

    gameWindow.draw(subtitle);

    // Рисуем пункты меню
    for (const auto& item : menuItems) {
        gameWindow.draw(item);
    }

    // Подсказки управления
    Text controlsHint;
    controlsHint.setFont(font);
    controlsHint.setString("Use ARROW KEYS to navigate, ENTER to select");
    controlsHint.setCharacterSize(18);
    controlsHint.setFillColor(Color(150, 150, 150));
    controlsHint.setPosition(20, WINDOW_HEIGHT - 40);

    gameWindow.draw(controlsHint);

    //Вывод всего золота
    Text totalGolds;
    totalGolds.setFont(font);
    totalGolds.setString("TotalGold:" + std::to_string(totalGoldValue));
    totalGolds.setCharacterSize(24);
    totalGolds.setFillColor(totalGold);
    totalGolds.setPosition(10, 10);

    gameWindow.draw(totalGolds);

    // Текущий выбор
    Text selectionHint;
    selectionHint.setFont(font);
    selectionHint.setString("Selected: " + menuItems[entityController.getSelectedMenuIndex()].getString());
    selectionHint.setCharacterSize(16);
    selectionHint.setFillColor(Color(100, 200, 100));
    selectionHint.setPosition(20, WINDOW_HEIGHT - 70);

    gameWindow.draw(selectionHint);

    gameWindow.display();
}

void GameMenu::updateMenuVisuals() {
    int selectedIndex = entityController.getSelectedMenuIndex();
    // Выводим только если индекс изменился
    if (selectedIndex != previousSelectedIndex) {
        std::cout << "=== SELECTION CHANGED ===" << std::endl;
        std::cout << "Previous: " << previousSelectedIndex << " -> New: " << selectedIndex << std::endl;
        std::cout << "Selected item: " << menuItems[selectedIndex].getString().toAnsiString() << std::endl;
        previousSelectedIndex = selectedIndex; // Обновляем предыдущее значение
    }

    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (i == selectedIndex) {
            menuItems[i].setFillColor(selectedColor);
            menuItems[i].setStyle(sf::Text::Bold);
        }
        else {
            menuItems[i].setFillColor(normalColor);
            menuItems[i].setStyle(sf::Text::Regular);
        }
    }
}

void GameMenu::handleMenuSelection(int selectedIndex) {
    //std::cout << "Menu item selected: " << selectedIndex << std::endl;
    std::cout << "=== MENU SELECTION ===" << std::endl;
    std::cout << "Selected index: " << selectedIndex << std::endl;
    std::cout << "Menu item: " << menuItems[selectedIndex].getString().toAnsiString() << std::endl;

    switch (selectedIndex) {
    case MenuItems::START_GAME:
        std::cout << "ACTION: Starting game..." << std::endl;
        isMenuActive = false;
        break;
    case MenuItems::NEW_GAME:
        std::cout << "ACTION: New starting game..." << std::endl;
        isMenuActive = false;
        resetGoldRequested = true;
        break;
    case MenuItems::SETTINGS:
        std::cout << "Settings selected" << std::endl;
        // Здесь можно открыть окно настроек
        break;

    case MenuItems::SHOP:
        std::cout << "Shop selected" << std::endl;
        // Здесь можно показать окно магазина
        break;

    case MenuItems::EXIT:
        std::cout << "ACTION: Exit selected" << std::endl;
        gameWindow.close();
        break;

    default:
        break;
    }
}
void GameMenu::resetMenu() {
    isMenuActive = true;
    entityController.resetMenuSelection();
    updateMenuVisuals();
}
