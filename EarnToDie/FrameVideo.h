#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class FrameVideo {
private:
    std::vector<sf::Texture> frames;
    sf::Clock clock;
    int currentFrame = 0;
    float frameDuration;
    float elapsedTime = 0.0f;
    bool loaded = false;
    bool looping = false;
    bool finished = false;

public:
    // Конструктор
    FrameVideo() : frameDuration(0.033f) {}

    // Загрузка кадров в формате finish_background_X.0.png
    bool loadFinishBackground(const std::string& folderPath, int startFrame, int endFrame) {
        frames.clear();
        loaded = false;

        for (int i = startFrame; i <= endFrame; i++) {
            sf::Texture frame;
            // Формат: finish_background_1.0.png, finish_background_2.0.png, etc.
            std::string filename = folderPath + "/finish_background_" + std::to_string(i) + ".png";

            if (frame.loadFromFile(filename)) {
                frames.push_back(frame);
                std::cout << "Loaded frame: " << filename << std::endl;
            }
            else {
                std::cout << "Failed to load frame: " << filename << std::endl;
                // Не прерываем загрузку, продолжаем со следующими кадрами
            }
        }

        loaded = !frames.empty();
        std::cout << "Successfully loaded " << frames.size() << " frames" << std::endl;
        return loaded;
    }

    // Установка FPS
    void setFPS(float fps) {
        if (fps > 0) {
            frameDuration = 1.0f / fps;
        }
    }

    // Включить/выключить зацикливание
    void setLooping(bool loop) {
        looping = loop;
    }

    // Обновление анимации
    void update() {
        if (!loaded || finished) return;

        elapsedTime += clock.restart().asSeconds();

        if (elapsedTime >= frameDuration) {
            currentFrame++;
            elapsedTime = 0.0f;

            // Проверка завершения
            if (currentFrame >= frames.size()) {
                if (looping) {
                    currentFrame = 0;
                }
                else {
                    currentFrame = frames.size() - 1;
                    finished = true;
                }
            }
        }
    }

    // Отрисовка текущего кадра
    void draw(sf::RenderWindow& window) {
        if (!loaded || currentFrame >= frames.size()) return;

        sf::Sprite sprite(frames[currentFrame]);

        // Масштабируем под размер окна
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = frames[currentFrame].getSize();

        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        sprite.setScale(scaleX, scaleY);

        window.draw(sprite);
    }

    // Отрисовка с позицией и масштабом
    void draw(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& scale) {
        if (!loaded || currentFrame >= frames.size()) return;

        sf::Sprite sprite(frames[currentFrame]);
        sprite.setPosition(position);
        sprite.setScale(scale);

        window.draw(sprite);
    }

    // Отрисовка с позицией, масштабом и поворотом
    void draw(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& scale, float rotation) {
        if (!loaded || currentFrame >= frames.size()) return;

        sf::Sprite sprite(frames[currentFrame]);
        sprite.setPosition(position);
        sprite.setScale(scale);
        sprite.setRotation(rotation);

        window.draw(sprite);
    }

    // Перезапуск видео
    void restart() {
        currentFrame = 0;
        elapsedTime = 0.0f;
        finished = false;
        clock.restart();
    }

    // Пропустить на определенный кадр
    void setCurrentFrame(int frame) {
        if (frame >= 0 && frame < frames.size()) {
            currentFrame = frame;
            elapsedTime = 0.0f;
        }
    }

    // Геттеры
    bool isLoaded() const { return loaded; }
    bool isFinished() const { return finished; }
    bool isLooping() const { return looping; }
    int getCurrentFrame() const { return currentFrame; }
    int getTotalFrames() const { return frames.size(); }
    float getFPS() const { return 1.0f / frameDuration; }
    float getProgress() const {
        return frames.empty() ? 0.0f : static_cast<float>(currentFrame) / frames.size();
    }

    // Получить текущую текстуру
    const sf::Texture* getCurrentTexture() const {
        if (!loaded || currentFrame >= frames.size()) return nullptr;
        return &frames[currentFrame];
    }

    // Получить размер текущего кадра
    sf::Vector2u getCurrentFrameSize() const {
        if (!loaded || currentFrame >= frames.size()) return sf::Vector2u(0, 0);
        return frames[currentFrame].getSize();
    }

    // Очистка ресурсов
    void clear() {
        frames.clear();
        loaded = false;
        currentFrame = 0;
        elapsedTime = 0.0f;
        finished = false;
    }
};