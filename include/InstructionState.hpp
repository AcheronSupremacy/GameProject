//
// Created by ACER on 4/8/2025.
//

#ifndef INSTRUCTIONSTATE_HPP
#define INSTRUCTIONSTATE_HPP
#pragma once
#include "GameState.hpp"
#include "Button.hpp"
#include "SDL_ttf.h"
#include <vector>
#include <string>

class InstructionState : public GameState {
public:
    InstructionState();

    void enter() override;
    void exit() override;
    void handleEvents(SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    TTF_Font* titleFont = nullptr;
    TTF_Font* instructionFont = nullptr;
    std::vector<Button> buttons;
    SDL_Texture* backgroundTexture = nullptr;

    struct InstructionText {
        std::string text;
        SDL_Texture* texture = nullptr;
        SDL_Rect position;
    };

    std::vector<InstructionText> instructions;

    void createInstructionTexts();
};
#endif //INSTRUCTIONSTATE_HPP
