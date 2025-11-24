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
    FrameVideo() : frameDuration(0.033f) {}

    bool loadFinishBackground(const std::string& folderPath, int startFrame, int endFrame) {
        frames.clear();
        loaded = false;

        for (int i = startFrame; i <= endFrame; i++) {
            sf::Texture frame;

            std::string filename = folderPath + "/finish_background_" + std::to_string(i) + ".png";

            if (frame.loadFromFile(filename)) {
                frames.push_back(frame);
                std::cout << "Loaded frame: " << filename << std::endl;
            }
            else {
                std::cout << "Failed to load frame: " << filename << std::endl;
            }
        }

        loaded = !frames.empty();
        std::cout << "Successfully loaded " << frames.size() << " frames" << std::endl;
        return loaded;
    }

    void setFPS(float fps) {
        if (fps > 0) {
            frameDuration = 1.0f / fps;
        }
    }

    void setLooping(bool loop) {
        looping = loop;
    }

    void update() {
        if (!loaded || finished) return;

        elapsedTime += clock.restart().asSeconds();

        if (elapsedTime >= frameDuration) {
            currentFrame++;
            elapsedTime = 0.0f;

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

    void draw(sf::RenderWindow& window) {
        if (!loaded || currentFrame >= frames.size()) return;

        sf::Sprite sprite(frames[currentFrame]);

        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = frames[currentFrame].getSize();

        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        sprite.setScale(scaleX, scaleY);

        window.draw(sprite);
    }

    void draw(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& scale) {
        if (!loaded || currentFrame >= frames.size()) return;

        sf::Sprite sprite(frames[currentFrame]);
        sprite.setPosition(position);
        sprite.setScale(scale);

        window.draw(sprite);
    }

    void draw(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& scale, float rotation) {
        if (!loaded || currentFrame >= frames.size()) return;

        sf::Sprite sprite(frames[currentFrame]);
        sprite.setPosition(position);
        sprite.setScale(scale);
        sprite.setRotation(rotation);

        window.draw(sprite);
    }

    void restart() {
        currentFrame = 0;
        elapsedTime = 0.0f;
        finished = false;
        clock.restart();
    }

    void setCurrentFrame(int frame) {
        if (frame >= 0 && frame < frames.size()) {
            currentFrame = frame;
            elapsedTime = 0.0f;
        }
    }
    bool isLoaded() const { return loaded; }
    bool isFinished() const { return finished; }
    bool isLooping() const { return looping; }
    int getCurrentFrame() const { return currentFrame; }
    int getTotalFrames() const { return frames.size(); }
    float getFPS() const { return 1.0f / frameDuration; }
    float getProgress() const {
        return frames.empty() ? 0.0f : static_cast<float>(currentFrame) / frames.size();
    }

    const sf::Texture* getCurrentTexture() const {
        if (!loaded || currentFrame >= frames.size()) return nullptr;
        return &frames[currentFrame];
    }

    sf::Vector2u getCurrentFrameSize() const {
        if (!loaded || currentFrame >= frames.size()) return sf::Vector2u(0, 0);
        return frames[currentFrame].getSize();
    }

    void clear() {
        frames.clear();
        loaded = false;
        currentFrame = 0;
        elapsedTime = 0.0f;
        finished = false;
    }
};