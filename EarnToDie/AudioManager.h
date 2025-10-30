#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager() = default;

    // Загрузка звуков
    bool loadSound(const std::string& name, const std::string& filename);
    bool loadMusic(const std::string& name, const std::string& filename);

    // Управление звуками
    void playSound(const std::string& name);
    void stopSound(const std::string& name);

    // Управление музыкой
    void playMusic(const std::string& name);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();

    // Настройки громкости
    void setSoundVolume(float volume);
    void setMusicVolume(float volume);
    float getSoundVolume() const { return soundVolume; }
    float getMusicVolume() const { return musicVolume; }

private:
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    std::unordered_map<std::string, sf::Music*> music;

    sf::Music* currentMusic = nullptr;
    float soundVolume = 70.0f;
    float musicVolume = 50.0f;
};