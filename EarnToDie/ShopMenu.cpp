#include "ShopMenu.h"
#include "const.h"
#include <iostream>
#include <fstream>

using namespace sf;

ShopMenu::ShopMenu(RenderWindow& window, int& goldRef, AudioManager& audioManager)
    : gameWindow(window)
    , playerGoldRef(goldRef)
    , audioManager(audioManager)
    , normalColor(Color::White)
    , selectedColor(Color::Yellow)
    , buttonColor(Color(70, 70, 70, 200))
    , buttonOutlineColor(Color::White)
    , titleColor(Color::Cyan)
    , priceColor(Color::Green)
    , isActive(false)
    , selectedIndex(-1)
    , playerGold(0)
    , previousSelectedIndex(-1) {

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
            throw std::runtime_error("Critical: Cannot load any font for shop menu!");
        }
    }

    initializeUpgrades();
    initializeMenuItems();
    initializeButtons();
    loadUpgrades();
}

void ShopMenu::initializeUpgrades() {
    upgrades = {
        {"Fuel", 100, 0, 5, " Capacity"},
        {"Boost", 150, 0, 5, " Power"},
        {"Speed", 200, 0, 5, " Upgrade"}
    };
}

void ShopMenu::initializeMenuItems() {
    float startY = WINDOW_HEIGHT / 2 - (upgrades.size() + 1) * (BUTTON_HEIGHT + BUTTON_PADDING) / 2 + 50;
    for (size_t i = 0; i < upgrades.size(); ++i) {
        Text itemText;
        itemText.setFont(font);
        itemText.setCharacterSize(22);
        itemText.setFillColor(normalColor);
        itemText.setString(upgrades[i].name + upgrades[i].description);

        FloatRect textRect = itemText.getLocalBounds();
        itemText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        itemText.setPosition(WINDOW_WIDTH / 2.0f - 120,
            startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        shopItems.push_back(itemText);

        Text priceText;
        priceText.setFont(font);
        priceText.setCharacterSize(20);
        priceText.setFillColor(priceColor);

        FloatRect priceRect = priceText.getLocalBounds();
        priceText.setOrigin(priceRect.left + priceRect.width / 2.0f,
            priceRect.top + priceRect.height / 2.0f);
        priceText.setPosition(WINDOW_WIDTH / 2.0f + 80,
            startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        priceTexts.push_back(priceText);


        Text levelText;
        levelText.setFont(font);
        levelText.setCharacterSize(18);
        levelText.setFillColor(Color(200, 200, 200));

        FloatRect levelRect = levelText.getLocalBounds();
        levelText.setOrigin(levelRect.left + levelRect.width / 2.0f,
            levelRect.top + levelRect.height / 2.0f);
        levelText.setPosition(WINDOW_WIDTH / 2.0f + 180,
            startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        levelTexts.push_back(levelText);
    }

    Text backText;
    backText.setFont(font);
    backText.setString("Back to Menu");
    backText.setCharacterSize(24);
    backText.setFillColor(normalColor);

    FloatRect backRect = backText.getLocalBounds();
    backText.setOrigin(backRect.left + backRect.width / 2.0f,
        backRect.top + backRect.height / 2.0f);
    backText.setPosition(WINDOW_WIDTH / 2.0f,
        startY + upgrades.size() * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

    shopItems.push_back(backText);
}

void ShopMenu::initializeButtons() {
    float startY = WINDOW_HEIGHT / 2 - (upgrades.size() + 1) * (BUTTON_HEIGHT + BUTTON_PADDING) / 2 + 50;

    for (size_t i = 0; i < upgrades.size() + 1; ++i) {
        RectangleShape button(Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
        button.setFillColor(buttonColor);
        button.setOutlineThickness(2.0f);
        button.setOutlineColor(buttonOutlineColor);

        button.setOrigin(BUTTON_WIDTH / 2.0f, BUTTON_HEIGHT / 2.0f);
        button.setPosition(WINDOW_WIDTH / 2.0f,
            startY + i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_HEIGHT / 2);

        buttons.push_back(button);
    }
}

bool ShopMenu::canAffordUpgrade(int index) const {
    if (index < 0 || index >= static_cast<int>(upgrades.size())) return false;
    return playerGoldRef >= getUpgradePrice(index) && upgrades[index].currentLevel < upgrades[index].maxLevel;
}

int ShopMenu::getUpgradePrice(int index) const {
    return upgrades[index].basePrice * (upgrades[index].currentLevel + 1);
}

std::string ShopMenu::getLevelText(int index) const {
    return "Lvl " + std::to_string(upgrades[index].currentLevel) + "/" + std::to_string(upgrades[index].maxLevel);
}

void ShopMenu::setActive(bool active) {
    isActive = active;
    if (active) {
        selectedIndex = -1;
        previousSelectedIndex = -1;
        std::cout << "Shop menu activated" << std::endl;
    }
}

void ShopMenu::update() {
    handleEvents();

    for (size_t i = 0; i < upgrades.size(); ++i) {
        if (upgrades[i].currentLevel >= upgrades[i].maxLevel) {
            priceTexts[i].setString("MAX");
            priceTexts[i].setFillColor(Color::Yellow);
        }
        else {
            priceTexts[i].setString(std::to_string(getUpgradePrice(i)) + "G");
            if (canAffordUpgrade(i)) {
                priceTexts[i].setFillColor(Color::Green);
            }
            else {
                priceTexts[i].setFillColor(Color::Red);
            }
        }
        levelTexts[i].setString(getLevelText(i));
    }

    for (size_t i = 0; i < buttons.size(); ++i) {
        if (i == selectedIndex) {
            buttons[i].setFillColor(Color(100, 100, 100, 200));
            buttons[i].setOutlineColor(selectedColor);
            buttons[i].setOutlineThickness(3.0f);
            if (i < shopItems.size()) {
                shopItems[i].setFillColor(selectedColor);
                shopItems[i].setStyle(Text::Bold);
                shopItems[i].setScale(1.03f, 1.03f);
            }
        }
        else {
            buttons[i].setFillColor(buttonColor);
            buttons[i].setOutlineColor(buttonOutlineColor);
            buttons[i].setOutlineThickness(2.0f);
            if (i < shopItems.size()) {
                shopItems[i].setFillColor(normalColor);
                shopItems[i].setStyle(Text::Regular);
                shopItems[i].setScale(1.0f, 1.0f);
            }
        }
    }
}

void ShopMenu::handleEvents() {
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
                    if (audioManager.isSoundLoaded("click")) {
                        audioManager.playSound("click");
                    }
                    std::cout << "Shop: UP to " << selectedIndex << std::endl;
                }
                break;
            case Keyboard::Down:
                if (selectedIndex < static_cast<int>(buttons.size()) - 1) {
                    selectedIndex++;
                    if (audioManager.isSoundLoaded("click")) {
                        audioManager.playSound("click");
                    }
                    std::cout << "Shop: DOWN to " << selectedIndex << std::endl;
                }
                break;
            case Keyboard::Return:
            case Keyboard::Space:
                if (selectedIndex >= 0) {
                    if (selectedIndex < static_cast<int>(upgrades.size())) {
                        if (canAffordUpgrade(selectedIndex)) {
                            int price = getUpgradePrice(selectedIndex);
                            playerGoldRef -= price;
                            upgrades[selectedIndex].currentLevel++;
                            std::cout << "Purchased " << upgrades[selectedIndex].name
                                << " level " << upgrades[selectedIndex].currentLevel
                                << " for " << price << " gold" << std::endl;
                            saveUpgrades();
                            applyUpgradesImmediately();
                        }
                    }
                    else {
                        std::cout << "Shop: BACK selected" << std::endl;
                        isActive = false;
                    }
                }
                break;
            case Keyboard::Escape:
                std::cout << "Shop: ESC pressed" << std::endl;
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
                        if (i < upgrades.size()) {
                            if (canAffordUpgrade(i)) {
                                int price = getUpgradePrice(i);
                                playerGoldRef -= price;
                                upgrades[i].currentLevel++;
                                std::cout << "Purchased " << upgrades[i].name
                                    << " level " << upgrades[i].currentLevel
                                    << " for " << price << " gold" << std::endl;
                                saveUpgrades();
                                applyUpgradesImmediately();
                            }
                        }
                        else {
                            isActive = false;
                        }
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

void ShopMenu::render() {
    if (!isActive) return;
    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(0, 0, 0, 180));
    gameWindow.draw(background);
    gameWindow.draw(overlay);

    Text title;
    title.setFont(font);
    title.setString("UPGRADE SHOP");
    title.setCharacterSize(50);
    title.setFillColor(titleColor);
    title.setStyle(Text::Bold);

    FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    title.setPosition(WINDOW_WIDTH / 2.0f, 100);
    gameWindow.draw(title);

    Text goldText;
    goldText.setFont(font);
    goldText.setString("Your Gold: " + std::to_string(playerGoldRef) + "G");
    goldText.setCharacterSize(28);
    goldText.setFillColor(Color::Yellow);
    goldText.setStyle(Text::Bold);

    FloatRect goldRect = goldText.getLocalBounds();
    goldText.setOrigin(goldRect.left + goldRect.width / 2.0f,
        goldRect.top + goldRect.height / 2.0f);
    goldText.setPosition(WINDOW_WIDTH / 2.0f, 160);
    gameWindow.draw(goldText);

    for (const auto& button : buttons) {
        gameWindow.draw(button);
    }

    for (const auto& text : shopItems) {
        gameWindow.draw(text);
    }

    for (const auto& priceText : priceTexts) {
        gameWindow.draw(priceText);
    }

    for (const auto& levelText : levelTexts) {
        gameWindow.draw(levelText);
    }

    Text controlsHint;
    controlsHint.setFont(font);
    controlsHint.setString("Click or use ARROW KEYS and ENTER to buy upgrades");
    controlsHint.setCharacterSize(16);
    controlsHint.setFillColor(Color(200, 200, 200));

    FloatRect hintRect = controlsHint.getLocalBounds();
    controlsHint.setOrigin(hintRect.left + hintRect.width / 2.0f,
        hintRect.top + hintRect.height / 2.0f);
    controlsHint.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 50);
    gameWindow.draw(controlsHint);
}

void ShopMenu::loadUpgrades() {
    std::ifstream file("shop_upgrades.dat");
    if (file.is_open()) {
        std::cout << "DEBUG: Loading shop upgrades from file..." << std::endl;
        for (size_t i = 0; i < upgrades.size(); ++i) {
            file >> upgrades[i].currentLevel;
            std::cout << "DEBUG: " << upgrades[i].name << " level: " << upgrades[i].currentLevel << std::endl;
        }
        file.close();
        std::cout << "DEBUG: Shop upgrades loaded successfully" << std::endl;

        std::cout << "DEBUG: Getters - Fuel: " << getFuelLevel()
            << ", Boost: " << getBoostLevel()
            << ", Speed: " << getSpeedLevel() << std::endl;
    }
    else {
        std::cout << "DEBUG: No shop upgrades file found, using defaults" << std::endl;
        for (auto& upgrade : upgrades) {
            upgrade.currentLevel = 0;
        }
    }
}

void ShopMenu::saveUpgrades() {
    std::ofstream file("shop_upgrades.dat");
    if (file.is_open()) {
        std::cout << "DEBUG: Saving shop upgrades..." << std::endl;
        for (const auto& upgrade : upgrades) {
            file << upgrade.currentLevel << " ";
            std::cout << "DEBUG: Saved " << upgrade.name << " level: " << upgrade.currentLevel << std::endl;
        }
        file.close();
        std::cout << "DEBUG: Shop upgrades saved successfully" << std::endl;
    }
    else {
        std::cout << "ERROR: Could not save shop upgrades!" << std::endl;
    }
}

void ShopMenu::applyUpgradesImmediately() {
    m_upgradesChanged = true;
    std::cout << "DEBUG: Upgrades marked as changed - need immediate application" << std::endl;
    std::cout << "DEBUG: Current levels - Fuel: " << getFuelLevel()
        << ", Boost: " << getBoostLevel()
        << ", Speed: " << getSpeedLevel() << std::endl;
}

void ShopMenu::resetUpgrades() {
    std::cout << "DEBUG: Resetting all shop upgrades to 0" << std::endl;
    for (auto& upgrade : upgrades) {
        std::cout << "DEBUG: Resetting " << upgrade.name << " from "
            << upgrade.currentLevel << " to 0" << std::endl;
        upgrade.currentLevel = 0;
    }
    saveUpgrades();
    std::cout << "DEBUG: All shop upgrades reset to 0 and saved" << std::endl;
}