//
// Created by ACER on 4/8/2025.
//
#include "SettingsState.hpp"
#include "Game.hpp"
#include "AudioManager.hpp"
#include "TextureManager.hpp"
#include <iostream>

SettingsState::SettingsState() = default;

void SettingsState::enter() {
    font = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 24);
    titleFont = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 60);
    if (!font || !titleFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    int w = game->getWindowWidth();
    int h = game->getWindowHeight();

    originalMusicVolume = AudioManager::getInstance().getMusicVolume();
    originalSfxVolume = AudioManager::getInstance().getEffectVolume();

    musicVolumeSlider = std::make_unique<Slider>(
        w/2 - 150, h/2 - 50, 300, 20,
        "Music Volume", font,
        SDL_Color{100, 100, 150, 255}, SDL_Color{200, 200, 255, 255}
    );

    sfxVolumeSlider = std::make_unique<Slider>(
        w/2 - 150, h/2 + 50, 300, 20,
        "SFX Volume", font,
        SDL_Color{100, 150, 100, 255}, SDL_Color{200, 255, 200, 255}
    );

    musicVolumeSlider->setValue(originalMusicVolume);
    sfxVolumeSlider->setValue(originalSfxVolume);

    if (font) {
        musicVolumeSlider->createLabelTexture(game->getRenderer(), font);
        sfxVolumeSlider->createLabelTexture(game->getRenderer(), font);
    }

    musicVolumeSlider->setCallback([this](int value) {
        AudioManager::getInstance().setMusicVolume(value);
    });

    sfxVolumeSlider->setCallback([this](int value) {
        AudioManager::getInstance().setEffectVolume(value);
    });

    buttons.emplace_back(w/2 - 200, h - 100, 180, 50, "Back",
                        SDL_Color{150, 50, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        AudioManager::getInstance().setMusicVolume(originalMusicVolume);
        AudioManager::getInstance().setEffectVolume(originalSfxVolume);

        game->getStateManager()->popState();
    });

    buttons.emplace_back(w/2 + 20, h - 100, 180, 50, "Apply",
                        SDL_Color{50, 150, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        originalMusicVolume = AudioManager::getInstance().getMusicVolume();
        originalSfxVolume = AudioManager::getInstance().getEffectVolume();

        game->getStateManager()->popState();
    });

    for (auto& button : buttons) {
        if (font) {
            button.setText(button.getText(), game->getRenderer(), font);
        }
    }


}

void SettingsState::exit() {
    buttons.clear();

    musicVolumeSlider.reset();
    sfxVolumeSlider.reset();

    if (titleFont) {
        TTF_CloseFont(titleFont);
        titleFont = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

void SettingsState::handleEvents(SDL_Event& e) {
    for (auto& button : buttons) {
        button.handleEvent(e);
    }

    if (musicVolumeSlider) {
        if (musicVolumeSlider->handleEvent(e) && font) {
            musicVolumeSlider->createLabelTexture(game->getRenderer(), font);
        }
    }

    if (sfxVolumeSlider) {
        if (sfxVolumeSlider->handleEvent(e) && font) {
            sfxVolumeSlider->createLabelTexture(game->getRenderer(), font);
        }
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        AudioManager::getInstance().setMusicVolume(originalMusicVolume);
        AudioManager::getInstance().setEffectVolume(originalSfxVolume);
        game->getStateManager()->popState();
    }
}

void SettingsState::update(float deltaTime) {
}

void SettingsState::render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
        SDL_RenderClear(renderer);

    if (titleFont) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* titleSurface = TTF_RenderText_Blended(titleFont, "Settings", textColor);
        if (titleSurface) {
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            int w = game->getWindowWidth();
            SDL_Rect titleRect = {w/2 - titleSurface->w/2, 50, titleSurface->w, titleSurface->h};
            SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
            SDL_FreeSurface(titleSurface);
            SDL_DestroyTexture(titleTexture);
        }
    }

    if (musicVolumeSlider) {
        musicVolumeSlider->render(renderer);
    }

    if (sfxVolumeSlider) {
        sfxVolumeSlider->render(renderer);
    }

    for (auto& button : buttons) {
        button.render(renderer);
    }
}