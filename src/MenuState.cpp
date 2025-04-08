//
// Created by ACER on 4/8/2025.
//

#include "MenuState.hpp"
#include "Game.hpp"
#include "SDL_ttf.h"
#include "TextureManager.hpp"
#include <iostream>

MenuState::MenuState() = default;


void MenuState::enter() {
    std::cout << "Entering Menu State" << std::endl;

    font = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }


    int w = game->getWindowWidth();
    int h = game->getWindowHeight();

    buttons.emplace_back(w/2 - 100, h/2 - 50, 200, 50, "Play",
                        SDL_Color{50, 150, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("play");
    });

    buttons.emplace_back(w/2 - 100, h/2 + 50, 200, 50, "Exit",
                        SDL_Color{150, 50, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        SDL_Event quitEvent;
        quitEvent.type = SDL_QUIT;
        SDL_PushEvent(&quitEvent);
    });

    // Setup button text
    for (auto& button : buttons) {
        button.setText(button.getText(), game->getRenderer(), font);
    }

    backgroundTexture = TextureManager::LoadTexture("assets/menu_background.png", game->getRenderer());
}

void MenuState::exit() {
    std::cout << "Exiting Menu State" << std::endl;
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
    if (backgroundTexture) {
        SDL_Rect dest = {0, 0, game->getWindowWidth(), game->getWindowHeight()};
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dest);
    } else {
        SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
        SDL_RenderClear(renderer);
    }

    if (font) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* titleSurface = TTF_RenderText_Blended(font, "2D Platformer", textColor);
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