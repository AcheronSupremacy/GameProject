//
// Created by ACER on 4/8/2025.
//
#include "AudioManager.hpp"
#include <SDL.h>
#include <iostream>

bool AudioManager::init() {
    if (initialized) {
        return true;
    }

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL Audio subsystem could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
    if ((Mix_Init(flags) & flags) != flags) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not open audio! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    initialized = true;
    return true;
}

AudioManager::~AudioManager() {
    cleanup();
}

void AudioManager::cleanup() {
    if (!initialized) return;

    for (auto& [id, music_ptr] : music) {
        if (music_ptr) {
            Mix_FreeMusic(music_ptr);
            music_ptr = nullptr;
        }
    }
    music.clear();

    for (auto& [id, chunk_ptr] : soundEffects) {
        if (chunk_ptr) {
            Mix_FreeChunk(chunk_ptr);
            chunk_ptr = nullptr;
        }
    }
    soundEffects.clear();

    Mix_CloseAudio();
    Mix_Quit();
    initialized = false;
}

bool AudioManager::loadMusic(const std::string& id, const std::string& path) {
    if (!initialized) return false;

    auto it = music.find(id);
    if (it != music.end() && it->second) {
        Mix_FreeMusic(it->second);
    }

    Mix_Music* musicPtr = Mix_LoadMUS(path.c_str());
    if (!musicPtr) {
        std::cerr << "Failed to load music " << path << "! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    music[id] = musicPtr;
    return true;
}

void AudioManager::playMusic(const std::string& id, int loops) {
    if (!initialized) return;

    auto it = music.find(id);
    if (it != music.end() && it->second) {
        Mix_PlayMusic(it->second, loops);
        Mix_VolumeMusic(MIX_MAX_VOLUME * musicVolume / 100);
    }
}

void AudioManager::pauseMusic() {
    if (!initialized) return;
    Mix_PauseMusic();
}

void AudioManager::resumeMusic() {
    if (!initialized) return;
    Mix_ResumeMusic();
}

void AudioManager::stopMusic() {
    if (!initialized) return;
    Mix_HaltMusic();
}

void AudioManager::setMusicVolume(int volume) {
    musicVolume = std::max(0, std::min(100, volume));
    if (initialized) {
        Mix_VolumeMusic(MIX_MAX_VOLUME * musicVolume / 100);
    }
}

int AudioManager::getMusicVolume() const {
    return musicVolume;
}

bool AudioManager::loadSoundEffect(const std::string& id, const std::string& path) {
    if (!initialized) return false;

    auto it = soundEffects.find(id);
    if (it != soundEffects.end() && it->second) {
        Mix_FreeChunk(it->second);
    }

    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        std::cerr << "Failed to load sound effect " << path << "! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    soundEffects[id] = chunk;
    Mix_VolumeChunk(chunk, MIX_MAX_VOLUME * effectVolume / 100);
    return true;
}

void AudioManager::playSoundEffect(const std::string& id, int loops, int channel) {
    if (!initialized) return;

    auto it = soundEffects.find(id);
    if (it != soundEffects.end() && it->second) {
        Mix_PlayChannel(channel, it->second, loops);
    }
}

void AudioManager::setEffectVolume(int volume) {
    effectVolume = std::max(0, std::min(100, volume));
    if (initialized) {
        for (auto& [id, chunk] : soundEffects) {
            if (chunk) {
                Mix_VolumeChunk(chunk, MIX_MAX_VOLUME * effectVolume / 100);
            }
        }
    }
}

int AudioManager::getEffectVolume() const {
    return effectVolume;
}