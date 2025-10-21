#include "GameMenu.h"
#include <iostream>
#include "const.h"

using namespace sf;
using namespace std;

GameMenu::GameMenu(RenderWindow& window, EntityController& controller):
    gameWindow(window),
    entityController(controller),
	normalColor(Color::White),
	selectedColor(Color::Yellow),
	titleColor(Color::Red),
    backgroundColor(sf::Color(30, 30, 60, 200)),
    isMenuActive(true),
    menuResult(MenuItems::START_GAME){

	if (!backgroundTexture.loadFromFile("menu_background.jpg")) {
		backgroundTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	background.setTexture(backgroundTexture);

	if (!font.loadFromFile("arial.ttf")) {
		std::cout << "Failed to load font! Using default." << std::endl;
		// В реальном проекте нужно обработать ошибку
	}
	initializeMenuItems();
}

void GameMenu::initializeMenuItems() {
	vector<string> menuTexts = {
		"Start Game",
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
                entityController.moveUp();
                updateMenuVisuals();
                break;
            case Keyboard::Down:
                entityController.moveDown();
                updateMenuVisuals();
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
                sf::FloatRect bounds = menuItems[i].getGlobalBounds();
                if (bounds.contains(static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y))) {
                    entityController.resetMenuSelection();
                    for (int j = 0; j < i; j++) {
                        entityController.moveDown();
                    }
                    updateMenuVisuals();
                }
            }
            break;

        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < menuItems.size(); ++i) {
                    sf::FloatRect bounds = menuItems[i].getGlobalBounds();
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

    sf::RectangleShape backgroundOverlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    backgroundOverlay.setFillColor(sf::Color(0, 0, 0, 180));
    gameWindow.draw(background);

    // Заголовок игры
    sf::Text title;
    title.setFont(font);
    title.setString("EARN TO DIE");
    title.setCharacterSize(70);
    title.setFillColor(titleColor);
    title.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    title.setPosition(WINDOW_WIDTH / 2.0f, 120);

    gameWindow.draw(title);

    // Подзаголовок
    sf::Text subtitle;
    subtitle.setFont(font);
    subtitle.setString("Autor: MrArs_Fok");
    subtitle.setCharacterSize(24);
    subtitle.setFillColor(sf::Color(200, 200, 200));

    sf::FloatRect subtitleRect = subtitle.getLocalBounds();
    subtitle.setOrigin(subtitleRect.left + subtitleRect.width / 2.0f,
        subtitleRect.top + subtitleRect.height / 2.0f);
    subtitle.setPosition(WINDOW_WIDTH / 2.0f, 180);

    gameWindow.draw(subtitle);

    // Рисуем пункты меню
    for (const auto& item : menuItems) {
        gameWindow.draw(item);
    }

    // Подсказки управления
    sf::Text controlsHint;
    controlsHint.setFont(font);
    controlsHint.setString("Use ARROW KEYS to navigate, ENTER to select");
    controlsHint.setCharacterSize(18);
    controlsHint.setFillColor(sf::Color(150, 150, 150));
    controlsHint.setPosition(20, WINDOW_HEIGHT - 40);

    gameWindow.draw(controlsHint);

    // Текущий выбор
    sf::Text selectionHint;
    selectionHint.setFont(font);
    selectionHint.setString("Selected: " + menuItems[entityController.getSelectedMenuIndex()].getString());
    selectionHint.setCharacterSize(16);
    selectionHint.setFillColor(sf::Color(100, 200, 100));
    selectionHint.setPosition(20, WINDOW_HEIGHT - 70);

    gameWindow.draw(selectionHint);

    gameWindow.display();
}

void GameMenu::updateMenuVisuals() {
    int selectedIndex = entityController.getSelectedMenuIndex();

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
    std::cout << "Menu item selected: " << selectedIndex << std::endl;

    switch (selectedIndex) {
    case MenuItems::START_GAME:
        std::cout << "Starting game..." << std::endl;
        isMenuActive = false;
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
        std::cout << "Exit selected" << std::endl;
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
