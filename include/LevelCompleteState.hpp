//
// Created by ACER on 4/8/2025.
//

#ifndef LEVELCOMPLETESTATE_HPP
#define LEVELCOMPLETESTATE_HPP
#pragma once
#include "GameState.hpp"
#include "Button.hpp"
#include "SDL_ttf.h"
#include <vector>

class LevelCompleteState : public GameState {
public:
    LevelCompleteState();

    void enter() override;
    void exit() override;
    void handleEvents(SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    TTF_Font* font = nullptr;
    std::vector<Button> buttons;
    SDL_Texture* overlayTexture = nullptr;
    SDL_Texture* completedTextTexture = nullptr;
    int completedTextWidth = 0;
    int completedTextHeight = 0;
};
#endif //LEVELCOMPLETESTATE_HPP
