#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool loadSound(const std::string& name, const std::string& filename);
    bool loadMusic(const std::string& name, const std::string& filename);

    void playSound(const std::string& name);
    void stopSound(const std::string& name);

    void playMusic(const std::string& name);
    void playLevelMusic(int level);
    void playMenuMusic();
    void pauseMusic();
    void resumeMusic();
    void stopMusic();

    void setSoundVolume(float volume);
    void setMusicVolume(float volume);
    float getSoundVolume() const { return soundVolume; }
    float getMusicVolume() const { return musicVolume; }

    bool isSoundLoaded(const std::string& name) const {
        return sounds.find(name) != sounds.end();
    }
    bool isMusicLoaded(const std::string& name) const {
        return music.find(name) != music.end();
    }

private:
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    std::unordered_map<std::string, sf::Music*> music;

    sf::Music* currentMusic = nullptr;
    float soundVolume = 70.0f;
    float musicVolume = 50.0f;
};