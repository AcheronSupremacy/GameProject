//
// Created by ACER on 4/8/2025.
//

#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP
#pragma once
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <memory>

class AudioManager {
public:
    static AudioManager& getInstance() {
        static AudioManager instance;
        return instance;
    }

    bool init();
    void cleanup();

    bool loadMusic(const std::string& id, const std::string& path);
    void playMusic(const std::string& id, int loops = -1);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
    void setMusicVolume(int volume);
    int getMusicVolume() const;

    bool loadSoundEffect(const std::string& id, const std::string& path);
    void playSoundEffect(const std::string& id, int loops = 0, int channel = -1);
    void setEffectVolume(int volume);
    int getEffectVolume() const;

private:
    AudioManager() = default;
    ~AudioManager();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    bool initialized = false;
    int musicVolume = 80;
    int effectVolume = 80;

    std::unordered_map<std::string, Mix_Music*> music;
    std::unordered_map<std::string, Mix_Chunk*> soundEffects;
};
#endif //AUDIOMANAGER_HPP
