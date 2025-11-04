#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager() {
    // Инициализация
}

bool AudioManager::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        std::cout << "Failed to load sound: " << filename << std::endl;
        return false;
    }

    soundBuffers[name] = buffer;
    sf::Sound sound;
    sound.setBuffer(soundBuffers[name]);
    sounds[name] = sound;

    std::cout << "Sound loaded: " << name << std::endl;
    return true;
}

bool AudioManager::loadMusic(const std::string& name, const std::string& filename) {
    sf::Music* music = new sf::Music();
    if (!music->openFromFile(filename)) {
        std::cout << "Failed to load music: " << filename << std::endl;
        delete music;
        return false;
    }

    this->music[name] = music;
    music->setLoop(true); // Музыка зацикливается
    music->setVolume(musicVolume);

    std::cout << "Music loaded: " << name << std::endl;
    return true;
}

void AudioManager::playSound(const std::string& name) {
    if (isSoundLoaded(name)) {
        sounds[name].setVolume(soundVolume);
        sounds[name].play();
    }
    else {
        std::cout << "Sound not loaded: " << name << std::endl;
    }
}

void AudioManager::stopSound(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].stop();
    }
}

void AudioManager::playMusic(const std::string& name) {
    if (music.find(name) != music.end()) {
        if (currentMusic) {
            currentMusic->stop();
        }
        currentMusic = music[name];
        currentMusic->setVolume(musicVolume);
        currentMusic->play();
    }
}

void AudioManager::pauseMusic() {
    if (currentMusic) {
        currentMusic->pause();
    }
}

void AudioManager::resumeMusic() {
    if (currentMusic) {
        currentMusic->play();
    }
}

void AudioManager::stopMusic() {
    if (currentMusic) {
        currentMusic->stop();
        currentMusic = nullptr;
    }
}

void AudioManager::setSoundVolume(float volume) {
    std::cout << "=== AudioManager: Setting sound volume to " << volume << " ===" << std::endl;
    soundVolume = volume;
    for (auto& sound : sounds) {
        sound.second.setVolume(soundVolume);
        std::cout << "Sound '" << sound.first << "' volume: " << sound.second.getVolume() << std::endl;
    }
}

void AudioManager::setMusicVolume(float volume) {
    std::cout << "=== AudioManager: Setting music volume to " << volume << " ===" << std::endl;
    musicVolume = volume;
    if (currentMusic) {
        currentMusic->setVolume(musicVolume);
        std::cout << "Current music volume: " << currentMusic->getVolume() << std::endl;
    }
}