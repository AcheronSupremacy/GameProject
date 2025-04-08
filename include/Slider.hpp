//
// Created by ACER on 4/8/2025.
//

#ifndef SLIDER_HPP
#define SLIDER_HPP
#pragma once

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>

class Slider {
public:
    Slider(int x, int y, int width, int height,
           const std::string& labelText,
           SDL_Color sliderColor, SDL_Color handleColor);
    ~Slider();

    void render(SDL_Renderer* renderer);
    bool handleEvent(SDL_Event& e);

    void setValue(int value);
    int getValue() const { return currentValue; }

    void setCallback(std::function<void(int)> callback) { valueChangedCallback = callback; }
    void createLabelTexture(SDL_Renderer* renderer, TTF_Font* font);
    void stopDragging();
private:
    SDL_Rect sliderRect;
    SDL_Rect handleRect;
    std::string label;
    SDL_Texture* labelTexture;
    SDL_Rect labelRect;

    SDL_Color sliderColor;
    SDL_Color handleColor;

    TTF_Font* font = nullptr;
    int currentValue = 50;
    bool isDragging = false;

    std::function<void(int)> valueChangedCallback = nullptr;

    void updateHandlePosition();

};
#endif //SLIDER_HPP