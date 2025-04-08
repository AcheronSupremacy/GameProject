//
// Created by ACER on 4/8/2025.
//
#include "GameOverState.hpp"
#include "Game.hpp"
#include "SDL_ttf.h"
#include <iostream>
#include <cmath>

GameOverState::GameOverState() = default;

void GameOverState::enter() {
    font = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 48);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    overlayTexture = SDL_CreateTexture(game->getRenderer(), SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET, game->getWindowWidth(), game->getWindowHeight());
    SDL_SetTextureBlendMode(overlayTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(game->getRenderer(), overlayTexture);
    SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 200);
    SDL_RenderClear(game->getRenderer());
    SDL_SetRenderTarget(game->getRenderer(), nullptr);

    SDL_Color textColor = {255, 50, 50, 255};
    SDL_Surface* gameoOverSurface = TTF_RenderText_Blended(font, "GAME OVER", textColor);
    if (gameoOverSurface) {
        gameOverTextTexture = SDL_CreateTextureFromSurface(game->getRenderer(), gameoOverSurface);
        gameOverTextWidth = gameoOverSurface->w;
        gameOverTextHeight = gameoOverSurface->h;
        SDL_FreeSurface(gameoOverSurface);
    }

    animationTime = 0.0f;
    animationComplete = false;

    int w = game->getWindowWidth();
    int h = game->getWindowHeight();

    buttons.emplace_back(w/2 - 100, h/2 + 100, 200, 50, "Restart Level",
                        SDL_Color{100, 100, 150, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("play");
    });

    buttons.emplace_back(w/2 - 100, h/2 + 170, 200, 50, "Main Menu",
                        SDL_Color{150, 50, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("menu");
    });

    for (auto& button : buttons) {
        button.setText(button.getText(), game->getRenderer(), font);
    }
}

void GameOverState::exit() {
    if (overlayTexture) {
        SDL_DestroyTexture(overlayTexture);
        overlayTexture = nullptr;
    }

    if (gameOverTextTexture) {
        SDL_DestroyTexture(gameOverTextTexture);
        gameOverTextTexture = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    buttons.clear();
}

void GameOverState::handleEvents(SDL_Event& e) {
    if (animationComplete) {
        for (auto& button : buttons) {
            button.handleEvent(e);
        }
    }
}

void GameOverState::update(float deltaTime) {
    if (!animationComplete) {
        animationTime += deltaTime;
        if (animationTime >= animationDuration) {
            animationTime = animationDuration;
            animationComplete = true;
        }
    }
}

void GameOverState::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, overlayTexture, nullptr, nullptr);

    if (gameOverTextTexture) {
        int w = game->getWindowWidth();
        int startY = -gameOverTextHeight; // Start off-screen
        int targetY = game->getWindowHeight() / 2 - gameOverTextHeight;

        float t = animationTime / animationDuration;
        float easeOut = 1 - pow(1 - t, 3); // Cubic ease-out formula

        int currentY = startY + (targetY - startY) * easeOut;

        SDL_Rect textRect = {
            w/2 - gameOverTextWidth/2,
            currentY,
            gameOverTextWidth,
            gameOverTextHeight
        };

        SDL_RenderCopy(renderer, gameOverTextTexture, nullptr, &textRect);
    }

    if (animationComplete) {
        for (auto& button : buttons) {
            button.render(renderer);
        }
    }
}