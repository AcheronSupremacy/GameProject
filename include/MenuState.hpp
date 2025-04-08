//
// Created by ACER on 4/8/2025.
//

#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP
#pragma once
#include "GameState.hpp"
#include "Button.hpp"
#include "SDL_ttf.h"
#include <vector>

class MenuState : public GameState {
public:
    MenuState();

    void enter() override;
    void exit() override;
    void handleEvents(SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    TTF_Font* font = nullptr;
    TTF_Font* titleFont = nullptr;
    std::vector<Button> buttons;
    SDL_Texture* backgroundTexture1 = nullptr;
    SDL_Texture* backgroundTexture2 = nullptr;
    SDL_Texture* backgroundTexture3 = nullptr;
};

#endif //MENUSTATE_HPP
