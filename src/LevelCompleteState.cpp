//
// Created by ACER on 4/8/2025.
//
#include "LevelCompleteState.hpp"
#include "Game.hpp"
#include "SDL_ttf.h"
#include <iostream>
#include <AudioManager.hpp>

LevelCompleteState::LevelCompleteState() = default;

void LevelCompleteState::enter() {
    font = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 24);
    titleFont = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 100);
    if (!font||!titleFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
    AudioManager::getInstance().stopMusic();
    AudioManager::getInstance().playSoundEffect("level_complete");

    SDL_Color textColor = {50, 255, 50, 255};
    SDL_Surface* completedSurface = TTF_RenderText_Blended(titleFont, "LEVEL CLEARED!", textColor);
    if (completedSurface) {
        completedTextTexture = SDL_CreateTextureFromSurface(game->getRenderer(), completedSurface);
        completedTextWidth = completedSurface->w;
        completedTextHeight = completedSurface->h;
        SDL_FreeSurface(completedSurface);
    }

    int w = game->getWindowWidth();
    int h = game->getWindowHeight();

    buttons.emplace_back(w/2 - 100, h/2 + 50, 200, 50, "Next Level",
                        SDL_Color{50, 150, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("play");
    });

    buttons.emplace_back(w/2 - 100, h/2 + 120, 200, 50, "Main Menu",
                        SDL_Color{150, 150, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("menu");
    });

    for (auto& button : buttons) {
        button.setText(button.getText(), game->getRenderer(), font);
    }
}

void LevelCompleteState::exit() {

    if (completedTextTexture) {
        SDL_DestroyTexture(completedTextTexture);
        completedTextTexture = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    buttons.clear();
}

void LevelCompleteState::handleEvents(SDL_Event& e) {
    for (auto& button : buttons) {
        button.handleEvent(e);
    }
}

void LevelCompleteState::update(float deltaTime) {
}

void LevelCompleteState::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); // Semi-transparent black
    SDL_Rect fullScreen = {0, 0, game->getWindowWidth(), game->getWindowHeight()};
    SDL_RenderFillRect(renderer, &fullScreen);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    if (completedTextTexture) {
        int w = game->getWindowWidth();
        SDL_Rect textRect = {
            w/2 - completedTextWidth/2,
            game->getWindowHeight()/2 - completedTextHeight - 50,
            completedTextWidth,
            completedTextHeight
        };
        SDL_RenderCopy(renderer, completedTextTexture, nullptr, &textRect);
    }

    for (auto& button : buttons) {
        button.render(renderer);
    }
}
