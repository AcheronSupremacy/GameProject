//
// Created by ACER on 4/8/2025.
//

#include "MenuState.hpp"
#include "Game.hpp"
#include "SDL_ttf.h"
#include "TextureManager.hpp"
#include <iostream>
#include "AudioManager.hpp"

MenuState::MenuState() = default;


void MenuState::enter() {
    AudioManager::getInstance().playMusic("bg_music");
    font = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 24);
    titleFont = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 100);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
    if (!titleFont) {
        std::cerr << "Failed to load title font: " << TTF_GetError() << std::endl;
    }

    int w = game->getWindowWidth();
    int h = game->getWindowHeight();

    buttons.emplace_back(w/2 - 100, h/2 - 50, 200, 50, "Play",
                        SDL_Color{50, 150, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("play");
    });

    buttons.emplace_back(w/2 - 100, h/2, 200, 50, "Instructions",
                      SDL_Color{50, 50, 150, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("instructions");
    });

    buttons.emplace_back(w/2 - 100, h/2 + 50, 200, 50, "Settings",
                      SDL_Color{150, 150, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("settings");
    });

    buttons.emplace_back(w/2 - 100, h/2 + 100, 200, 50, "Exit",
                        SDL_Color{150, 50, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        SDL_Event quitEvent;
        quitEvent.type = SDL_QUIT;
        SDL_PushEvent(&quitEvent);
    });

    for (auto& button : buttons) {
        button.setText(button.getText(), game->getRenderer(), font);
    }

    backgroundTexture1 = TextureManager::LoadTexture("assets/background/background_layer_1.png", game->getRenderer());
    backgroundTexture2 = TextureManager::LoadTexture("assets/background/background_layer_2.png", game->getRenderer());
    backgroundTexture3 = TextureManager::LoadTexture("assets/background/background_layer_3.png", game->getRenderer());
}

void MenuState::exit() {
    buttons.clear();
}

void MenuState::handleEvents(SDL_Event& e) {
    for (auto& button : buttons) {
        button.handleEvent(e);
    }
}

void MenuState::update(float deltaTime) {
}

void MenuState::render(SDL_Renderer* renderer) {
    if (backgroundTexture1&&backgroundTexture2&&backgroundTexture3) {
        SDL_Rect dest = {0, 0, game->getWindowWidth(), game->getWindowHeight()};
        SDL_RenderCopy(renderer, backgroundTexture1, nullptr, &dest);
        SDL_RenderCopy(renderer, backgroundTexture2, nullptr, &dest);
        SDL_RenderCopy(renderer, backgroundTexture3, nullptr, &dest);
    } else {
        SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
        SDL_RenderClear(renderer);
    }

    if (titleFont) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* titleSurface = TTF_RenderText_Blended(titleFont, "Skill Issue?", textColor);
        if (titleSurface) {
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            int w = game->getWindowWidth();
            SDL_Rect titleRect = {w/2 - titleSurface->w/2, 100, titleSurface->w, titleSurface->h};
            SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
            SDL_FreeSurface(titleSurface);
            SDL_DestroyTexture(titleTexture);
        }
    }

    for (auto& button : buttons) {
        button.render(renderer);
    }
}