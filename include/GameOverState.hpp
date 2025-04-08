//
// Created by ACER on 4/8/2025.
//

#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP
#pragma once
#include "GameState.hpp"
#include "Button.hpp"
#include "SDL_ttf.h"
#include <vector>

class GameOverState : public GameState {
public:
    GameOverState();

    void enter() override;
    void exit() override;
    void handleEvents(SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    TTF_Font* font = nullptr;
    TTF_Font* titleFont = nullptr;
    std::vector<Button> buttons;
    SDL_Texture* overlayTexture = nullptr;

    // Animation properties
    float animationTime = 0.0f;
    float animationDuration = 1.5f;
    bool animationComplete = false;
    SDL_Texture* gameOverTextTexture = nullptr;
    int gameOverTextWidth = 0;
    int gameOverTextHeight = 0;
};
#endif //GAMEOVERSTATE_HPP
