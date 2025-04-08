//
// Created by ACER on 4/8/2025.
//
#include "GameOverState.hpp"
#include "Game.hpp"
#include "PlayState.hpp"
#include "SDL_ttf.h"
#include <iostream>
#include <cmath>
#include "AudioManager.hpp"
GameOverState::GameOverState() = default;

void GameOverState::enter() {
    font = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 24);
    titleFont = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 100);
    if (!font||!titleFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
    AudioManager::getInstance().stopMusic();
    AudioManager::getInstance().playSoundEffect("game_over");

    SDL_Color textColor = {255, 50, 50, 255};
    SDL_Surface* gameOverSurface = TTF_RenderText_Blended(titleFont, "GAME OVER", textColor);
    if (gameOverSurface) {
        gameOverTextTexture = SDL_CreateTextureFromSurface(game->getRenderer(), gameOverSurface);
        gameOverTextWidth = gameOverSurface->w;
        gameOverTextHeight = gameOverSurface->h;
        SDL_FreeSurface(gameOverSurface);
    }

    animationTime = 0.0f;
    animationComplete = false;

    int w = game->getWindowWidth();
    int h = game->getWindowHeight();

    buttons.emplace_back(w/2 - 100, h/2 + 100, 200, 50, "Restart Level",
                        SDL_Color{100, 100, 150, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        AudioManager::getInstance().playMusic("bg_music", -1);
        game->getStateManager()->clearAndSetState("play");
    });

    buttons.emplace_back(w/2 - 100, h/2 + 170, 200, 50, "Main Menu",
                        SDL_Color{150, 50, 50, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->clearAndSetState("menu");
    });

    for (auto& button : buttons) {
        button.setText(button.getText(), game->getRenderer());
    }
}

void GameOverState::exit() {
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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect fullScreen = {0, 0, game->getWindowWidth(), game->getWindowHeight()};
    SDL_RenderFillRect(renderer, &fullScreen);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    if (gameOverTextTexture) {
        int w = game->getWindowWidth();
        int startY = -gameOverTextHeight;
        int targetY = game->getWindowHeight() / 2 - gameOverTextHeight;

        float t = animationTime / animationDuration;
        float easeOut = 1 - pow(1 - t, 3);

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