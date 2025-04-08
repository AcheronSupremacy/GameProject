//
// Created by ACER on 4/8/2025.
//

#ifndef SETTINGSTATE_HPP
#define SETTINGSTATE_HPP
#pragma once

#include "GameState.hpp"
#include "Button.hpp"
#include "Slider.hpp"
#include "SDL_ttf.h"
#include <vector>

class SettingsState : public GameState {
public:
    SettingsState();

    void enter() override;
    void exit() override;
    void handleEvents(SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    TTF_Font* font = nullptr;
    TTF_Font* titleFont = nullptr;
    std::vector<Button> buttons;

    std::unique_ptr<Slider> musicVolumeSlider;
    std::unique_ptr<Slider> sfxVolumeSlider;

    SDL_Texture* backgroundTexture = nullptr;
};

#endif //SETTINGSTATE_HPP
