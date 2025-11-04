#include <SFML/Graphics.hpp>
#include "EntityController.h"
#include "const.h"
#include "EnemyController.h"
#include "ObjectController.h"
#include "CircleObject.h"
#include "RectObject.h"
#include "EnemySpawner.h"
#include "SubjectSpawner.h"
#include "FinalGameWindow.h"
#include "main.h"
#include "GameState.h"
#include "GameMenu.h"
#include "ShopMenu.h"
#include "PauseMenu.h"
#include "AudioManager.h"
#include "SettingsMenu.h"
#include "LevelMenu.h"
#include <vector>
#include <ctime>
#include <memory>
#include <cstdlib>

using namespace sf;
using namespace std;

void resetGame(GameState& gameState, ShopMenu& shopMenu, vector<unique_ptr<EnemyController>>& enemies, vector<unique_ptr<ObjectController>>& subjects, EntityController& controller,
    float& enemySpawnTimer, float& subjectSpawnTimer) {
    cout << "DEBUG: resetGame called" << endl;
    shopMenu.loadUpgrades();
    gameState.restartGame();
    gameState.resetDistance();
    enemies.clear();
    subjects.clear();
    enemySpawnTimer = 0.0f;
    subjectSpawnTimer = 0.0f;

    controller.applyShopUpgrades();
    gameState.applyShopUpgrades();
}

void handlePauseMode(PauseMenu& pauseMenu, GameState& gameState, EntityController& controller,
    vector<unique_ptr<EnemyController>>& enemies, vector<unique_ptr<ObjectController>>& subjects,
    float& enemySpawnTimer, float& subjectSpawnTimer) {

    pauseMenu.resetMenuResult();

    while (pauseMenu.isActive() && pauseMenu.getWindow().isOpen()) {
        pauseMenu.update();

        int menuResult = pauseMenu.getPauseMenuResult();

        switch (menuResult) {
        case PauseMenuItems::RESUME:
            pauseMenu.setActive(false);
            pauseMenu.setGamePaused(false);
            break;
        case PauseMenuItems::MAIN_MENU:
            controller.setReturnToMainMenu();
            pauseMenu.setActive(false);
            pauseMenu.setGamePaused(false);
            break;
        case PauseMenuItems::EXIT:
            pauseMenu.getWindow().close();
            return;
        default:
            break;
        }

        pauseMenu.render();
        pauseMenu.getWindow().display();

        if (!pauseMenu.isActive()) {
            break;
        }
    }
}

void handleFinalGameWindow(FinalGameWindow& finalWindow, GameMenu& menu, GameState& gameState, ShopMenu& shopMenu,
    vector<unique_ptr<EnemyController>>& enemies, vector<unique_ptr<ObjectController>>& subjects,
    EntityController& controller, float& enemySpawnTimer, float& subjectSpawnTimer, RenderWindow& window) {
    finalWindow.update();
    finalWindow.render();
    finalWindow.getWindow().display();

    if (!finalWindow.isActiveState()) {
        int action = finalWindow.getSelectedAction();
        switch (action) {
        case FinalGameWindow::RESTART_GAME:
            cout << "Restarting game from final window..." << endl;
            resetGame(gameState, shopMenu, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer);
            break;
        case FinalGameWindow::MAIN_MENU:
            cout << "Returning to main menu from final window..." << endl;
            menu.setActive(true);
            resetGame(gameState, shopMenu, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer);
            break;
        case FinalGameWindow::EXIT_GAME:
            cout << "Exiting game from final window..." << endl;
            window.close();
            break;
        }
    }
}

int main()
{
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "EarnToDie");

    window.setFramerateLimit(120);
    window.setVerticalSyncEnabled(false);

    GameState gameState;
    AudioManager audioManager;
    LevelMenu levelMenu(window, audioManager);
    ShopMenu shopMenu(window, gameState.getTotalGoldRef());
    EntityController controller(gameState, &shopMenu);
    gameState.setShopMenu(&shopMenu);
    FinalGameWindow finalWindow(window);

    gameState.loadGold();
    shopMenu.loadUpgrades();
    controller.applyShopUpgrades();
    gameState.applyShopUpgrades();

    GameMenu menu(window, audioManager, gameState.getTotalGoldRef(), shopMenu, levelMenu);
    PauseMenu pauseMenu(window, audioManager);
    SettingsMenu settingsMenu(window, audioManager);

    Texture backgroundTexture;

    audioManager.loadMusic("background", "assets/music/background.ogg");
    audioManager.loadSound("click", "assets/sounds/click.wav");
    audioManager.loadSound("collision", "assets/sounds/collision.wav");
    audioManager.loadSound("coin", "assets/sounds/coin.wav");

    audioManager.playMusic("background");

    if (!backgroundTexture.loadFromFile("background.jpg")) {
        cout << "Failed to load background image!" << endl;
    }
    Sprite background1(backgroundTexture);
    Sprite background2(backgroundTexture);

    Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(WINDOW_WIDTH) / textureSize.x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / textureSize.y;

    background1.setScale(scaleX, scaleY);
    background2.setScale(scaleX, scaleY);

    background1.setPosition(0, 0);
    background2.setPosition(0, -static_cast<float>(WINDOW_HEIGHT));

    float backgroundSpeed = 600.0f;
    float backgroundY1 = 0.0f;
    float backgroundY2 = -static_cast<float>(WINDOW_HEIGHT);

    vector<unique_ptr<EnemyController>> enemies;
    vector<unique_ptr<ObjectController>> subjects;

    const float enemySpawnRate = 1.0f;
    const float subjectSpawnRate = 10.0f;

    float enemySpawnTimer = 0.0f;
    float subjectSpawnTimer = 0.0f;
    float distanceTimer = 0.0f;
    float fuelTimer = 0.0f;
    float saveTimer = 0.0f;

    float speed = 0;
    Clock gameClock;

    SubjectSpawner subjectSpawner(subjectSpawnTimer, subjectSpawnRate, subjects, gameState, 1.5f);
    EnemySpawner enemySpawner(enemySpawnTimer, enemySpawnRate, enemies, gameState, 1.5f);

    gameState.loadGold();

    while (window.isOpen()) {
        // Главное меню и подменю
        if (menu.isActive() || menu.isSettingsActive() || menu.isShopActive() || levelMenu.isActive()) {
            if (levelMenu.isActive()) {
                levelMenu.update();
                levelMenu.render();

                // Проверяем выбор уровня
                int selectedLevel = levelMenu.getSelectedLevel();
                if (selectedLevel > 0) {
                    cout << "Starting level " << selectedLevel << "..." << endl;
                    resetGame(gameState, shopMenu, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer);

                    // Устанавливаем целевое расстояние в зависимости от уровня
                    switch (selectedLevel) {
                    case 1:
                        gameState.setTargetDistance(150);
                        break;
                    case 2:
                        gameState.setTargetDistance(300);
                        break;
                    case 3:
                        gameState.setTargetDistance(500);
                        break;
                    case 4:
                        gameState.setTargetDistance(700);
                        break;
                    case 5:
                        gameState.setTargetDistance(1000);
                        break;
                    }
                    levelMenu.setActive(false);
                    levelMenu.resetSelection();
                }
                else if (selectedLevel == 0 && !levelMenu.isActive()) {
                    // Пользователь вернулся в главное меню
                    menu.setActive(true);
                }
                continue;
            }
            else {
                // Обработка главного меню и других подменю
                menu.update();
                menu.render();

                if (!menu.isShopActive() && shopMenu.isActiveState()) {
                    shopMenu.setActive(false);
                    std::cout << "=== SHOP CLOSED ===" << std::endl;
                    shopMenu.loadUpgrades();
                    controller.applyShopUpgrades();
                    gameState.applyShopUpgrades();
                    std::cout << "=== UPGRADES APPLIED AFTER SHOP ===" << std::endl;
                }

                if (menu.isResetGoldRequested()) {
                    menu.resetShopUpgrades();
                    shopMenu.loadUpgrades();
                    gameState.resetGold();
                    controller.applyShopUpgrades();
                    gameState.applyShopUpgrades();
                    menu.clearResetGoldRequest();
                }
                continue;
            }
        }

        // Final Game Window
        if (finalWindow.isActiveState()) {
            handleFinalGameWindow(finalWindow, menu, gameState, shopMenu, enemies, subjects, controller,
                enemySpawnTimer, subjectSpawnTimer, window);
            continue;
        }

        // Возврат в главное меню
        if (controller.shouldReturnToMainMenu()) {
            controller.resetReturnToMainMenu();
            menu.setActive(true);
            resetGame(gameState, shopMenu, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer);
            continue;
        }

        // Обработка событий окна
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                break;
            }

            // Обработка ESC для входа в паузу
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                if (!pauseMenu.isActive() && gameState.isPlaying() && !controller.isGameFinal()) {
                    pauseMenu.setGamePaused(true);
                    pauseMenu.setActive(true);
                    cout << "PAUSE: Game paused via PauseMenu" << endl;
                }
            }
        }

        // Синхронизация состояния паузы
        if (pauseMenu.isGamePaused() != gameState.isPaused()) {
            gameState.setPaused(pauseMenu.isGamePaused());
        }

        // Обработка рестарта игры
        if (controller.shouldRestartGame()) {
            resetGame(gameState, shopMenu, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer);
            controller.resetRestartFlag();
            controller.applyShopUpgrades();
            gameState.applyShopUpgrades();
        }

        // Режим паузы
        if (pauseMenu.isActive() || pauseMenu.isGamePaused() || pauseMenu.isSettingsActive()) {
            handlePauseMode(pauseMenu, gameState, controller, enemies, subjects, enemySpawnTimer, subjectSpawnTimer);

            // Проверяем результат после выхода из режима паузы
            if (controller.shouldReturnToMainMenu()) {
                controller.resetReturnToMainMenu();
                menu.setActive(true);
                resetGame(gameState, shopMenu, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer);
                continue;
            }
            continue;
        }

        // Проверка условий окончания игры
        if (gameState.isGameWon()) {
            // Проверяем, что цель действительно была установлена
            if (gameState.isTargetSet()) {
                finalWindow.setActive(true, 1); // 1 = победа
                continue;
            }
            else {
                cout << "WARNING: Game won but no target was set! Resetting..." << endl;
                gameState.restartGame(); // Перезапускаем игру
            }
        }
        if (gameState.isGameOver()) {
            finalWindow.setActive(true, 0); // 0 = поражение
            continue;
        }
        if (controller.isGameFinal()) {
            bool playerWon = controller.hasPlayerWon();
            finalWindow.setActive(true, playerWon ? 1 : 0);
            continue;
        }

        window.clear();

        // Основная игровая логика (только когда игра не на паузе)
        if (gameState.isPlaying() && !pauseMenu.isGamePaused() && !controller.isGameFinal()) {
            if (shopMenu.haveUpgradesChanged()) {
                std::cout << "=== MAIN: APPLYING UPGRADES ===" << std::endl;
                controller.applyShopUpgrades();
                gameState.applyShopUpgrades();
                shopMenu.clearUpgradesChanged();
                std::cout << "=== MAIN: UPGRADES APPLIED ===" << std::endl;
            }

            float deltaTime = gameClock.restart().asSeconds();
            float scaledDeltaTime = deltaTime * controller.getGameSpeed();
            speed = controller.getGameSpeed();
            gameState.decreaseSpeed(speed);

            fuelTimer += deltaTime;
            if (fuelTimer >= 1.0f) {
                int fuel = static_cast<int>(speed);
                gameState.decreaseFuel(fuel);
                fuelTimer = 0.0f;
            }

            float movement = backgroundSpeed * scaledDeltaTime;
            backgroundY1 += movement;
            backgroundY2 += movement;

            distanceTimer += deltaTime;
            if (distanceTimer >= 1.0f) {
                int dist = static_cast<int>(speed * 3);
                distanceTimer = 0.0f;
                gameState.decreaseDist(dist);

                if (gameState.getPlayerDist() > 100) {
                    enemySpawner.setSpawnRate(0.5f);
                    enemySpawner.setSpeedMultiplier(2.0f);
                }
            }

            if (backgroundY1 >= WINDOW_HEIGHT) {
                backgroundY1 = backgroundY2 - WINDOW_HEIGHT;
            }
            if (backgroundY2 >= WINDOW_HEIGHT) {
                backgroundY2 = backgroundY1 - WINDOW_HEIGHT;
            }

            background1.setPosition(0, backgroundY1);
            background2.setPosition(0, backgroundY2);

            enemySpawnTimer += scaledDeltaTime;
            subjectSpawnTimer += scaledDeltaTime;

            subjectSpawner.update();
            enemySpawner.update();

            // Обновление врагов
            for (auto it = enemies.begin(); it != enemies.end();) {
                bool shouldRemove = (*it)->update();

                if (!shouldRemove && (*it)->getEnemy()->checkCollision(controller.getEntity()->shape)) {
                    gameState.addGold(10);
                    it = enemies.erase(it);
                    continue;
                }
                if (shouldRemove) {
                    it = enemies.erase(it);
                }
                else {
                    ++it;
                }
            }

            // Обновление объектов
            for (auto it = subjects.begin(); it != subjects.end();) {
                bool shouldRemove = (*it)->update();

                if (shouldRemove) {
                    it = subjects.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        // Отрисовка
        window.draw(background1);
        window.draw(background2);

        for (const auto& subject : subjects) {
            subject->draw(window);
        }
        for (const auto& enemy : enemies) {
            enemy->draw(window);
        }

        gameState.draw(window);
        controller.update(window);

        window.display();
    }

    gameState.saveGold();
    return 0;
}