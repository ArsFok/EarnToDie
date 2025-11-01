#include "SettingsMenu.h"
#include "MenuController.h"
#include <iostream>
#include <fstream> 
#include <sstream> 
#include <filesystem>
#include "const.h"

SettingsMenu::SettingsMenu(sf::RenderWindow& window, AudioManager& audioManager)
    : gameWindow(window)
    , audioManager(audioManager)
    , menuController(4) // 4 пункта меню: Music, Sound, Save, Back
    , normalColor(sf::Color::White)
    , selectedColor(sf::Color::Yellow)
    , buttonColor(sf::Color(70, 70, 70, 200))
    , buttonOutlineColor(sf::Color::White)
    , titleColor(sf::Color::Cyan)
    , valueColor(sf::Color::Green)
    , previousSelectedIndex(-1) {

    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Failed to load font for settings menu!" << std::endl;
    }
    if (!backgroundTexture.loadFromFile("menu_background.png")) { // Тот же фон что в ShopMenu
        backgroundTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    background.setTexture(backgroundTexture);

    loadSettings();

    initializeMenuItems();
    initializeButtons();
    setActive(false);
}

void SettingsMenu::setActive(bool active) {
    isMenuActive = active;
    if (active) {
        menuController.resetSelection();
        updateValueTexts();
        updateMenuVisuals();
    }
}

void SettingsMenu::initializeMenuItems() {
    std::vector<std::string> menuTexts = {
        "Music Volume",
        "Sound Volume",
        "Save Settings",
        "Back to Menu"
    };

    menuController.setMenuItemsCount(menuTexts.size());

    float startY = WINDOW_HEIGHT / 2 - (menuTexts.size() * (BUTTON_HEIGHT + BUTTON_PADDING)) / 2 + 50;

    for (size_t i = 0; i < menuTexts.size(); ++i) {
        // Текст пункта меню
        sf::Text text;
        text.setFont(font);
        text.setString(menuTexts[i]);
        text.setCharacterSize(22); // Такой же размер как в ShopMenu
        text.setFillColor(normalColor);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        // Позиционируем как в ShopMenu
        text.setPosition(WINDOW_WIDTH / 2.0f - 120,
            startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        menuSettingsItems.push_back(text);

        // Текст значения настройки
        sf::Text valueText;
        valueText.setFont(font);
        valueText.setCharacterSize(20); // Такой же размер как в ShopMenu
        valueText.setFillColor(valueColor);

        sf::FloatRect valueRect = valueText.getLocalBounds();
        valueText.setOrigin(valueRect.left + valueRect.width / 2.0f,
            valueRect.top + valueRect.height / 2.0f);
        // Позиционируем как в ShopMenu
        valueText.setPosition(WINDOW_WIDTH / 2.0f + 80,
            startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        valueTexts.push_back(valueText);
    }
}

void SettingsMenu::initializeButtons() {
    float startY = WINDOW_HEIGHT / 2 - (menuSettingsItems.size() * (BUTTON_HEIGHT + BUTTON_PADDING)) / 2 + 50;

    for (size_t i = 0; i < menuSettingsItems.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
        button.setFillColor(buttonColor);
        button.setOutlineThickness(2.0f);
        button.setOutlineColor(buttonOutlineColor);

        button.setOrigin(BUTTON_WIDTH / 2.0f, BUTTON_HEIGHT / 2.0f);
        button.setPosition(WINDOW_WIDTH / 2.0f,
            startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        buttons.push_back(button);
    }
}

void SettingsMenu::updateValueTexts() {
    // Проверяем, что векторы имеют достаточный размер
    if (valueTexts.size() > SettingsMenuItems::MUSIC_VOLUME) {
        valueTexts[SettingsMenuItems::MUSIC_VOLUME].setString(std::to_string(static_cast<int>(musicVolume)) + "%");
    }
    if (valueTexts.size() > SettingsMenuItems::SOUND_VOLUME) {
        valueTexts[SettingsMenuItems::SOUND_VOLUME].setString(std::to_string(static_cast<int>(soundVolume)) + "%");
    }
    // Для SAVE_SETTINGS и BACK оставляем пустые строки
    if (valueTexts.size() > SettingsMenuItems::SAVE_SETTINGS) {
        valueTexts[SettingsMenuItems::SAVE_SETTINGS].setString("");
    }
    if (valueTexts.size() > SettingsMenuItems::BACK) {
        valueTexts[SettingsMenuItems::BACK].setString("");
    }
}

void SettingsMenu::update() {
    handleEvents();
    updateMenuVisuals();
}

void SettingsMenu::handleEvents() {
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
                    std::cout << "=== SETTINGS MENU ===" << std::endl;
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
                    std::cout << "=== SETTINGS MENU ===" << std::endl;
                    std::cout << "DOWN: " << before << " -> " << after << std::endl;
                }
            }
            break;

            case sf::Keyboard::PageDown:
                adjustSetting(-10);
                break;

            case sf::Keyboard::PageUp:
                adjustSetting(10);
                break;

            case sf::Keyboard::Home:
            {
                int selectedIndex = menuController.getSelectedIndex();
                if (selectedIndex == SettingsMenuItems::MUSIC_VOLUME) {
                    musicVolume = 0.0f;
                }
                else if (selectedIndex == SettingsMenuItems::SOUND_VOLUME) {
                    soundVolume = 0.0f;
                }
                updateValueTexts();
            }
            break;

            case sf::Keyboard::End:
            {
                int selectedIndex = menuController.getSelectedIndex();
                if (selectedIndex == SettingsMenuItems::MUSIC_VOLUME) {
                    musicVolume = 100.0f;
                }
                else if (selectedIndex == SettingsMenuItems::SOUND_VOLUME) {
                    soundVolume = 100.0f;
                }
                updateValueTexts();
            }
            break;

            case sf::Keyboard::Return:
            case sf::Keyboard::Space:
            {
                int selectedIndex = menuController.getSelectedIndex();
                if (menuController.isValidIndex()) {
                    std::cout << "=== SETTINGS MENU ACTION ===" << std::endl;
                    std::cout << "ENTER/SPACE pressed on: " << selectedIndex << std::endl;
                    handleMenuSelection(selectedIndex);
                }
            }
            break;

            case sf::Keyboard::Escape:
                std::cout << "=== SETTINGS MENU ===" << std::endl;
                std::cout << "ESC pressed - returning to previous menu" << std::endl;
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
    handleContinuousInput();
}

void SettingsMenu::handleContinuousInput() {
    static sf::Clock keyTimer;
    static bool keyProcessed = false;
    static bool wasKeyPressed = false;

    // Настройки задержки
    const float initialDelay = 500.0f;   // Первое нажатие - 0.5 секунды
    const float repeatDelay = 150.0f;    // Повторения - 0.15 секунды

    bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Num0);

    bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Num9);

    // Если нажаты обе - игнорируем
    if (leftPressed && rightPressed) {
        keyProcessed = false;
        wasKeyPressed = false;
        return;
    }

    float elapsedTime = keyTimer.getElapsedTime().asMilliseconds();
    bool anyKeyPressed = leftPressed || rightPressed;

    // Если клавиша только что нажата
    if (anyKeyPressed && !wasKeyPressed) {
        // Немедленно обрабатываем первое нажатие
        if (leftPressed) adjustSetting(-1);
        if (rightPressed) adjustSetting(1);

        keyTimer.restart();
        keyProcessed = true;
        wasKeyPressed = true;
    }
    // Если клавиша удерживается
    else if (anyKeyPressed && wasKeyPressed) {
        // Обрабатываем только если прошло достаточно времени
        if (elapsedTime > (keyProcessed ? repeatDelay : initialDelay)) {
            if (leftPressed) adjustSetting(-1);
            if (rightPressed) adjustSetting(1);

            keyTimer.restart();
            keyProcessed = true;
        }
    }
    // Если клавиша отпущена
    else {
        keyProcessed = false;
        wasKeyPressed = false;
    }
}

void SettingsMenu::adjustSetting(int direction) {
    int selectedIndex = menuController.getSelectedIndex();

    switch (selectedIndex) {
    case SettingsMenuItems::MUSIC_VOLUME:
        musicVolume = std::max(0.0f, std::min(100.0f, musicVolume + direction * 5.0f));
        std::cout << "Music volume: " << musicVolume << "%" << std::endl;
        audioManager.setMusicVolume(musicVolume);
        updateValueTexts();
        saveSettings();

        // Визуальная обратная связь - мигание текста
        if (menuSettingsItems.size() > selectedIndex) {
            menuSettingsItems[selectedIndex].setFillColor(sf::Color::Green);
        }
        break;

    case SettingsMenuItems::SOUND_VOLUME:
        soundVolume = std::max(0.0f, std::min(100.0f, soundVolume + direction * 5.0f));
        std::cout << "Sound volume: " << soundVolume << "%" << std::endl;
        audioManager.setSoundVolume(soundVolume);
        updateValueTexts();
        saveSettings();

        // Визуальная обратная связь - мигание текста
        if (menuSettingsItems.size() > selectedIndex) {
            menuSettingsItems[selectedIndex].setFillColor(sf::Color::Green);
        }
        break;

    default:
        break;
    }
}

void SettingsMenu::updateMenuVisuals() {
    int selectedIndex = menuController.getSelectedIndex();

    if (!menuController.isValidIndex()) {
        return;
    }

    if (selectedIndex != previousSelectedIndex) {
        std::cout << "=== SETTINGS MENU SELECTION ===" << std::endl;
        std::cout << "Changed: " << previousSelectedIndex << " -> " << selectedIndex << std::endl;
        std::cout << "Now selected: " << menuSettingsItems[selectedIndex].getString().toAnsiString() << std::endl;
        previousSelectedIndex = selectedIndex;
    }

    for (size_t i = 0; i < menuSettingsItems.size(); ++i) {
        if (i >= buttons.size()) continue;

        if (i == selectedIndex) {
            buttons[i].setFillColor(sf::Color(100, 100, 100, 200));
            buttons[i].setOutlineColor(selectedColor);
            buttons[i].setOutlineThickness(3.0f);
            menuSettingsItems[i].setFillColor(selectedColor);
            menuSettingsItems[i].setStyle(sf::Text::Bold);
            menuSettingsItems[i].setScale(1.03f, 1.03f); // Такой же масштаб как в ShopMenu
        }
        else {
            buttons[i].setFillColor(buttonColor);
            buttons[i].setOutlineColor(buttonOutlineColor);
            buttons[i].setOutlineThickness(2.0f);
            menuSettingsItems[i].setFillColor(normalColor);
            menuSettingsItems[i].setStyle(sf::Text::Regular);
            menuSettingsItems[i].setScale(1.0f, 1.0f);
        }
    }
}

void SettingsMenu::handleMenuSelection(int selectedIndex) {
    if (!menuController.isValidIndex() || selectedIndex >= menuSettingsItems.size()) {
        return;
    }

    std::cout << "=== SETTINGS MENU FINAL SELECTION ===" << std::endl;
    std::cout << "Selected: " << selectedIndex << " - " << menuSettingsItems[selectedIndex].getString().toAnsiString() << std::endl;

    settingsMenuResult = selectedIndex;

    switch (selectedIndex) {
    case SettingsMenuItems::MUSIC_VOLUME:
    case SettingsMenuItems::SOUND_VOLUME:
        adjustSetting(1);
        break;
    case SettingsMenuItems::SAVE_SETTINGS:
        saveSettings();
        audioManager.playSound("click");
        menuSettingsItems[selectedIndex].setString("Settings Saved!");
        break;
    case SettingsMenuItems::BACK:
        std::cout << "ACTION: Returning to previous menu..." << std::endl;
        setActive(false);
        break;
    default:
        break;
    }
}

void SettingsMenu::render() {
    // Полупрозрачный темный фон (как в ShopMenu)
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    gameWindow.draw(background);
    gameWindow.draw(overlay);

    // Заголовок настроек
    sf::Text settingsTitle;
    settingsTitle.setFont(font);
    settingsTitle.setString("SETTINGS");
    settingsTitle.setCharacterSize(50); // Такой же размер как в ShopMenu
    settingsTitle.setFillColor(titleColor);
    settingsTitle.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = settingsTitle.getLocalBounds();
    settingsTitle.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    settingsTitle.setPosition(WINDOW_WIDTH / 2.0f, 100); // Такая же позиция как в ShopMenu
    gameWindow.draw(settingsTitle);

    // Рисуем кнопки
    for (const auto& button : buttons) {
        gameWindow.draw(button);
    }

    // Рисуем текст пунктов меню
    for (const auto& text : menuSettingsItems) {
        gameWindow.draw(text);
    }

    // Рисуем тексты значений
    for (const auto& valueText : valueTexts) {
        gameWindow.draw(valueText);
    }

    // Подсказки управления
    sf::Text controlsHint;
    controlsHint.setFont(font);
    controlsHint.setString("ARROWS/A/D/0/9: Adjust volume  |  PAGE UP/DOWN: Quick adjust  |  HOME/END: Min/Max  |  ENTER: Confirm  |  ESC: Back");
    controlsHint.setCharacterSize(16); // Немного больше для лучшей читаемости
    controlsHint.setFillColor(sf::Color(200, 200, 200));

    sf::FloatRect hintRect = controlsHint.getLocalBounds();
    controlsHint.setOrigin(hintRect.left + hintRect.width / 2.0f,
        hintRect.top + hintRect.height / 2.0f);
    controlsHint.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 50); // Такая же позиция как в ShopMenu
    gameWindow.draw(controlsHint);
}

std::string SettingsMenu::getSettingsFilePath() {
    return "game_settings.cfg";
}

void SettingsMenu::saveSettings() {
    std::string filePath = getSettingsFilePath();
    std::ofstream file(filePath);

    if (file.is_open()) {
        file << "music_volume=" << static_cast<int>(musicVolume) << std::endl;
        file << "sound_volume=" << static_cast<int>(soundVolume) << std::endl;
        file.close();

        std::cout << "Settings saved successfully to: " << filePath << std::endl;
        std::cout << "Music: " << static_cast<int>(musicVolume) << "%, Sound: " << static_cast<int>(soundVolume) << "%" << std::endl;
    }
    else {
        std::cout << "ERROR: Could not create settings file: " << filePath << std::endl;
        std::cout << "Make sure the program has write permissions in this directory." << std::endl;
    }
}

void SettingsMenu::loadSettings() {
    std::string filePath = getSettingsFilePath();
    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;
        bool loadedMusic = false;
        bool loadedSound = false;

        while (std::getline(file, line)) {
            // Убираем пробелы в начале и конце строки
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            // Пропускаем пустые строки и комментарии
            if (line.empty() || line[0] == '#') continue;

            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);

                // Убираем пробелы вокруг ключа и значения
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                try {
                    if (key == "music_volume") {
                        float vol = std::stof(value);
                        musicVolume = std::max(0.0f, std::min(100.0f, vol));
                        loadedMusic = true;
                        std::cout << "Loaded music volume: " << musicVolume << "%" << std::endl;
                    }
                    else if (key == "sound_volume") {
                        float vol = std::stof(value);
                        soundVolume = std::max(0.0f, std::min(100.0f, vol));
                        loadedSound = true;
                        std::cout << "Loaded sound volume: " << soundVolume << "%" << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error parsing setting '" << key << "': " << e.what() << std::endl;
                }
            }
        }
        file.close();

        if (!loadedMusic || !loadedSound) {
            std::cout << "Some settings were not found in file, using defaults for missing ones" << std::endl;
            if (!loadedMusic) musicVolume = 50.0f;
            if (!loadedSound) soundVolume = 70.0f;
        }

    }
    else {
        std::cout << "No settings file found at '" << filePath << "'. Using default values." << std::endl;
        musicVolume = 50.0f;
        soundVolume = 70.0f;

        std::cout << "Attempting to create settings file..." << std::endl;
        saveSettings();
    }

    audioManager.setMusicVolume(musicVolume);
    audioManager.setSoundVolume(soundVolume);

    std::cout << "Applied settings to AudioManager - Music: " << musicVolume
        << "%, Sound: " << soundVolume << "%" << std::endl;

    updateValueTexts();
}