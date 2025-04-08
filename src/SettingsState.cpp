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

    musicVolumeSlider = std::make_unique<Slider>(
        w/2 + 100, h/2 - 50, 300, 20,
        "Music Volume", font,
        SDL_Color{100, 100, 150, 255}, SDL_Color{200, 200, 255, 255}
    );

    sfxVolumeSlider = std::make_unique<Slider>(
        w/2 + 100, h/2 + 50, 300, 20,
        "SFX Volume", font,
        SDL_Color{100, 150, 100, 255}, SDL_Color{200, 255, 200, 255}
    );

    musicVolumeSlider->setValue(AudioManager::getInstance().getMusicVolume());
    sfxVolumeSlider->setValue(AudioManager::getInstance().getEffectVolume());

    musicVolumeSlider->setCallback([](int value) {
        AudioManager::getInstance().setMusicVolume(value);
    });

    sfxVolumeSlider->setCallback([](int value) {
        AudioManager::getInstance().setEffectVolume(value);
    });

    buttons.emplace_back(w/2 - 100, h - 100, 200, 50, "Back",
                        SDL_Color{150, 50, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->popState();
    });

    buttons.emplace_back(w/2 - 100, h - 170, 200, 50, "Apply",
                        SDL_Color{50, 150, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {});

    for (auto& button : buttons) {
        button.setText(button.getText(), game->getRenderer(), font);
    }

    backgroundTexture = TextureManager::LoadTexture("assets/background/background_layer_1.png", game->getRenderer());
}

void SettingsState::exit() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (titleFont) {
        TTF_CloseFont(titleFont);
        titleFont = nullptr;
    }

    buttons.clear();
    musicVolumeSlider.reset();
    sfxVolumeSlider.reset();
}

void SettingsState::handleEvents(SDL_Event& e) {
    for (auto& button : buttons) {
        button.handleEvent(e);
    }

    musicVolumeSlider->handleEvent(e);
    sfxVolumeSlider->handleEvent(e);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        game->getStateManager()->popState();
    }
}

void SettingsState::update(float deltaTime) {
}

void SettingsState::render(SDL_Renderer* renderer) {
    if (backgroundTexture) {
        SDL_Rect dest = {0, 0, game->getWindowWidth(), game->getWindowHeight()};
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dest);
    } else {
        SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
        SDL_RenderClear(renderer);
    }

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

    musicVolumeSlider->render(renderer);
    sfxVolumeSlider->render(renderer);

    for (auto& button : buttons) {
        button.render(renderer);
    }
}