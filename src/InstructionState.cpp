//
// Created by ACER on 4/8/2025.
//
//
// Created by ACER on 4/8/2025.
//

#include "InstructionState.hpp"
#include "Game.hpp"
#include "SDL_ttf.h"
#include <iostream>

InstructionState::InstructionState() = default;

void InstructionState::enter() {
    titleFont = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 100);
    if (!titleFont) {
        std::cerr << "Failed to load title font: " << TTF_GetError() << std::endl;
    }

    instructionFont = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 24);
    if (!instructionFont) {
        std::cerr << "Failed to load instruction font: " << TTF_GetError() << std::endl;
    }


    int w = game->getWindowWidth();
    int h = game->getWindowHeight();

    buttons.emplace_back(w/2 - 100, h - 100, 200, 50, "Back",
                        SDL_Color{100, 100, 150, 255}, SDL_Color{255, 255, 255, 255});
    buttons.back().setCallback([this]() {
        game->getStateManager()->changeState("menu");
    });

    for (auto& button : buttons) {
        button.setText(button.getText(), game->getRenderer(), instructionFont);
    }

    createInstructionTexts();
}

void InstructionState::createInstructionTexts() {
    if (!instructionFont) return;

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Renderer* renderer = game->getRenderer();
    int w = game->getWindowWidth();

    std::vector<std::string> instructionStrings = {
        "A/D Keys: Move",
        "Space: Jump",
        "Left Shift: Dash",
        "Esc: Pause Game",
        "",
        "Wall slide by touching walls while falling",
        "Press Space while wall sliding to wall jump",
        "You can dash only once every 1 second"
    };

    int yPos = 200;
    for (const auto& text : instructionStrings) {
        InstructionText instruction;
        instruction.text = text;

        if (!text.empty()) {
            SDL_Surface* surface = TTF_RenderText_Blended(instructionFont, text.c_str(), textColor);
            if (surface) {
                instruction.texture = SDL_CreateTextureFromSurface(renderer, surface);
                instruction.position = {w/2 - surface->w/2, yPos, surface->w, surface->h};
                SDL_FreeSurface(surface);
            }
        }

        yPos += text.empty() ? 20 : 40;
        instructions.push_back(instruction);
    }
}

void InstructionState::exit() {

    if (titleFont) {
        TTF_CloseFont(titleFont);
        titleFont = nullptr;
    }

    if (instructionFont) {
        TTF_CloseFont(instructionFont);
        instructionFont = nullptr;
    }

    for (auto& instruction : instructions) {
        if (instruction.texture) {
            SDL_DestroyTexture(instruction.texture);
            instruction.texture = nullptr;
        }
    }
    instructions.clear();

    buttons.clear();
}

void InstructionState::handleEvents(SDL_Event& e) {
    for (auto& button : buttons) {
        button.handleEvent(e);
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        game->getStateManager()->changeState("menu");
    }
}

void InstructionState::update(float deltaTime) {
}

void InstructionState::render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
        SDL_RenderClear(renderer);

    if (titleFont) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* titleSurface = TTF_RenderText_Blended(titleFont, "Instructions", textColor);
        if (titleSurface) {
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            int w = game->getWindowWidth();
            SDL_Rect titleRect = {w/2 - titleSurface->w/2, 50, titleSurface->w, titleSurface->h};
            SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
            SDL_FreeSurface(titleSurface);
            SDL_DestroyTexture(titleTexture);
        }
    }

    for (const auto& instruction : instructions) {
        if (instruction.texture) {
            SDL_RenderCopy(renderer, instruction.texture, nullptr, &instruction.position);
        }
    }

    for (auto& button : buttons) {
        button.render(renderer);
    }
}