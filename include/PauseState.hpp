//
// Created by ACER on 4/8/2025.
//

#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#pragma once
#include "GameState.hpp"
#include "Button.hpp"
#include "SDL_ttf.h"
#include <vector>

class PauseState : public GameState {
public:
    PauseState();

    void enter() override;
    void exit() override;
    void handleEvents(SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    TTF_Font* font = nullptr;
    std::vector<Button> buttons;
    SDL_Texture* overlayTexture = nullptr;
};
#endif //PAUSESTATE_HPP
