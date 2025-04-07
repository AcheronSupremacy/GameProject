//
// Created by ACER on 4/7/2025.
//

#ifndef BUTTON_HPP
#define BUTTON_HPP

#pragma once
#include <SDL.h>
#include <string>
#include <functional>
#include "SDL_ttf.h"

class Button {
public:
    Button(int x, int y, int w, int h,
           const std::string& text = "",
           SDL_Color bgColor = {70, 70, 70, 255},
           SDL_Color textColor = {255, 255, 255, 255});

    bool handleEvent(const SDL_Event& e);

    void render(SDL_Renderer* renderer);

    void setColors(SDL_Color bgNormal, SDL_Color bgHover, SDL_Color bgPressed);
    void setText(const std::string& text, SDL_Renderer* renderer, TTF_Font* font);

    void setCallback(std::function<void()> onClick);

private:
    SDL_Rect rect;
    std::string text;

    SDL_Color bgNormal = {70, 70, 70, 255};
    SDL_Color bgHover = {100, 100, 100, 255};
    SDL_Color bgPressed = {50, 50, 50, 255};
    SDL_Color textColor = {255, 255, 255, 255};

    enum State { NORMAL, HOVERED, PRESSED };
    State currentState = NORMAL;

    std::function<void()> onClick = nullptr;

    SDL_Texture* textTexture = nullptr;

};
#endif //BUTTON_HPP
