#include <SFML/Graphics.hpp>
#include "EntityController.h"
#include "const.h"
#include "EnemyController.h"
#include "ObjectController.h"
#include "CircleObject.h"
#include "RectObject.h"
#include "main.h"
#include "GameState.h"
#include "GameMenu.h"
#include "PauseMenu.h"
#include "AudioManager.h"
#include "SettingsMenu.h"
#include <vector>
#include <ctime>
#include <memory>
#include <cstdlib>

using namespace sf;
using namespace std;

void createEnemies(float& enemySpawnTimer, const float enemySpawnRate, vector<unique_ptr<EnemyController>>& enemies, GameState& gameState, float new_speed) {
    if (!gameState.isPlaying()) return;
    if (enemySpawnTimer >= enemySpawnRate) {
        enemySpawnTimer = 0.0f;

        float speed = new_speed;
        int gold = 1;
        float size = 70;
        const int spawnWidth = WINDOW_WIDTH - 485;
        const int minX = 240;

        Vector2f position(minX + rand() % spawnWidth, -60.f);

        enemies.push_back(
            make_unique<EnemyController>(
                make_unique<RectObject>(size, position, gold),
                new_speed
            )
        );
    }
}

void createSubject(float& enemySpawnTimer, const float enemySpawnRate, vector<unique_ptr<ObjectController>>& subjects, GameState& gameState, float new_speed) {
    if (!gameState.isPlaying()) return;
    if (enemySpawnTimer >= enemySpawnRate) {
        enemySpawnTimer = 0.0f;

        float speed = new_speed;

        Vector2f position(240 + rand() % (WINDOW_WIDTH - 495), -60);
        float radius = 30;

        unique_ptr<StaticObject> subject = make_unique<CircleObject>(radius, position);
        subjects.push_back(make_unique<ObjectController>(move(subject), speed));
    }
}

void resetGame(GameState& gameState, vector<unique_ptr<EnemyController>>& enemies, vector<unique_ptr<ObjectController>>& subjects, EntityController& controller,
    float& enemySpawnTimer, float& subjectSpawnTimer, int& distance) {
    gameState.restartGame();
    enemies.clear();
    subjects.clear();
    enemySpawnTimer = 0.0f;
    subjectSpawnTimer = 0.0f;
    distance = 0;
}

void handlePauseMode(PauseMenu& pauseMenu, GameState& gameState, EntityController& controller,
    vector<unique_ptr<EnemyController>>& enemies, vector<unique_ptr<ObjectController>>& subjects,
    float& enemySpawnTimer, float& subjectSpawnTimer, int& distance) {

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

int main()
{
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "EarnToDie");

    GameState gameState;
    EntityController controller;
    AudioManager audioManager;

    GameMenu menu(window, audioManager);
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

    float backgroundSpeed = 400.0f;
    float backgroundY1 = 0.0f;
    float backgroundY2 = -static_cast<float>(WINDOW_HEIGHT);

    vector<unique_ptr<EnemyController>> enemies;
    vector<unique_ptr<ObjectController>> subjects;

    float enemySpawnRate = 1.0f;
    const float subjectSpawnRate = 1.5f;

    float enemySpawnTimer = 0.0f;
    float subjectSpawnTimer = 0.0f;
    float distanceTimer = 0.0f;
    float fuelTimer = 0.0f;
    float saveTimer = 0.0f;

    float speed = 0;
    int dist = 0;
    int distance = 0;
    Clock gameClock;

    gameState.loadGold();

    while (window.isOpen()) {
        // Главное меню
        if (menu.isActive() || menu.isSettingsActive()) {
            menu.update();
            menu.render();
            menu.setTotalGold(gameState.getTotalGold());

            if (!menu.isActive() && menu.getMenuResult() == MenuItems::START_GAME) {
                gameState.restartGame();
                resetGame(gameState, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer, distance);
            }
            continue;
        }

        if (menu.isResetGoldRequested()) {
            gameState.resetGold();
            menu.clearResetGoldRequest();
        }

        if (controller.shouldReturnToMainMenu()) {
            controller.resetReturnToMainMenu();
            menu.setActive(true);
            resetGame(gameState, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer, distance);
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
                    // Включаем паузу через PauseMenu
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
            resetGame(gameState, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer, dist);
            controller.resetRestartFlag();
        }

        // Режим паузы
        if (pauseMenu.isActive() || pauseMenu.isGamePaused() || pauseMenu.isSettingsActive()) {
            handlePauseMode(pauseMenu, gameState, controller, enemies, subjects, enemySpawnTimer, subjectSpawnTimer, distance);

            // Проверяем результат после выхода из режима паузы
            if (controller.shouldReturnToMainMenu()) {
                controller.resetReturnToMainMenu();
                menu.setActive(true);
                resetGame(gameState, enemies, subjects, controller, enemySpawnTimer, subjectSpawnTimer, distance);
                continue;
            }

            continue;
        }

        window.clear();

        // Основная игровая логика (только когда игра не на паузе)
        if (gameState.isPlaying() && !pauseMenu.isGamePaused() && !controller.isGameFinal()) {
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
                dist = speed * distanceTimer;
                distanceTimer = 0.0f;
                gameState.decreaseDist(dist);
                distance += dist;
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

            if (distance > 100) {
                enemySpawnRate = 0.5f;
            }

            createEnemies(enemySpawnTimer, enemySpawnRate, enemies, gameState, speed);
            createSubject(subjectSpawnTimer, subjectSpawnRate, subjects, gameState, speed);

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

        for (const auto& enemy : enemies) {
            enemy->draw(window);
        }
        for (const auto& subject : subjects) {
            subject->draw(window);
        }

        gameState.draw(window);
        controller.update(window);

        window.display();
    }

    gameState.saveGold();
    return 0;
}