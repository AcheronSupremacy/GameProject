//
// Created by ACER on 4/8/2025.
//
// src/PauseState.cpp
#include "PauseState.hpp"
#include "Game.hpp"
#include "SDL_ttf.h"
#include <iostream>

PauseState::PauseState() =default;




void PauseState::enter() {

    font = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    overlayTexture = SDL_CreateTexture(game->getRenderer(), SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET, game->getWindowWidth(), game->getWindowHeight());
    SDL_SetTextureBlendMode(overlayTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(game->getRenderer(), overlayTexture);
    SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
    SDL_RenderClear(game->getRenderer());
    SDL_SetRenderTarget(game->getRenderer(), nullptr);

    int w = game->getWindowWidth();
    int h = game->getWindowHeight();

    buttons.emplace_back(w/2 - 100, h/2 - 50, 200, 50, "Resume",
                        SDL_Color{50, 150, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("play");
    });

    buttons.emplace_back(w/2 - 100, h/2 + 50, 200, 50, "Main Menu",
                        SDL_Color{150, 150, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("menu");
    });

    for (auto& button : buttons) {
        button.setText(button.getText(), game->getRenderer(), font);
    }
}

void PauseState::exit() {
    if (overlayTexture) {
        SDL_DestroyTexture(overlayTexture);
        overlayTexture = nullptr;
    }
    buttons.clear();
}

void PauseState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        game->getStateManager()->changeState("play");
        return;
    }

    for (auto& button : buttons) {
        button.handleEvent(e);
    }
}

void PauseState::update(float deltaTime) {
}

void PauseState::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, overlayTexture, nullptr, nullptr);

    if (font) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* titleSurface = TTF_RenderText_Blended(font, "PAUSED", textColor);
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