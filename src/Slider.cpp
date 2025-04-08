//
// Created by ACER on 4/8/2025.
//
#include "Slider.hpp"
#include <iostream>

Slider::Slider(int x, int y, int width, int height,
               const std::string& labelText, TTF_Font* font,
               SDL_Color sliderColor, SDL_Color handleColor)
    : sliderRect{x, y, width, height},
      handleRect{x, y - height/2, height*2, height*2},
      label(labelText),
      font(font),
      sliderColor(sliderColor),
      handleColor(handleColor),
      labelTexture(nullptr) {

    updateHandlePosition();
}


Slider::~Slider() {
    if (labelTexture) {
        SDL_DestroyTexture(labelTexture);
        labelTexture = nullptr;
    }
}

void Slider::render(SDL_Renderer* renderer) {
    if (!labelTexture && !label.empty()) {
        createLabelTexture(renderer, TTF_OpenFont("assets/PixelifySans-Regular.ttf", 20));
    }

    SDL_SetRenderDrawColor(renderer, sliderColor.r, sliderColor.g, sliderColor.b, sliderColor.a);
    SDL_RenderFillRect(renderer, &sliderRect);

    SDL_SetRenderDrawColor(renderer, handleColor.r, handleColor.g, handleColor.b, handleColor.a);
    SDL_RenderFillRect(renderer, &handleRect);

    if (labelTexture) {
        SDL_RenderCopy(renderer, labelTexture, nullptr, &labelRect);
    }
}

bool Slider::handleEvent(SDL_Event& e) {
    bool valueChanged = false;

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int x = e.button.x;
        int y = e.button.y;
        SDL_Point point = {x, y};
        if (SDL_PointInRect(&point, &handleRect) ||
            SDL_PointInRect(&point, &sliderRect)) {
            isDragging = true;

            int newValue = 100 * (x - sliderRect.x) / sliderRect.w;
            if (newValue != currentValue) {
                currentValue = std::max(0, std::min(100, newValue));
                updateHandlePosition();
                createLabelTexture(SDL_GetRenderer(SDL_GetMouseFocus()), font);
                valueChanged = true;
            }
            }
    }
    else if (e.type == SDL_MOUSEMOTION && isDragging) {
        int x = e.motion.x;

        int newValue = 100 * (x - sliderRect.x) / sliderRect.w;
        if (newValue != currentValue) {
            currentValue = std::max(0, std::min(100, newValue));
            updateHandlePosition();
            createLabelTexture(SDL_GetRenderer(SDL_GetMouseFocus()), font);
            valueChanged = true;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
        isDragging = false;
    }

    if (valueChanged && valueChangedCallback) {
        valueChangedCallback(currentValue);
    }

    return valueChanged;
}

void Slider::setValue(int value) {
    currentValue = std::max(0, std::min(100, value));
    updateHandlePosition();
}

void Slider::updateHandlePosition() {
    int handleX = sliderRect.x + (sliderRect.w * currentValue / 100) - (handleRect.w / 2);
    handleRect.x = handleX;
}

void Slider::createLabelTexture(SDL_Renderer* renderer, TTF_Font* font) {
    if (labelTexture) {
        SDL_DestroyTexture(labelTexture);
        labelTexture = nullptr;
    }
    if (!font||!renderer) {
        return;
    }
    std::string displayText = label + ": " + std::to_string(currentValue) + "%";

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, displayText.c_str(), textColor);
    if (surface) {
        labelTexture = SDL_CreateTextureFromSurface(renderer, surface);

        labelRect.x = sliderRect.x - surface->w - 20;
        labelRect.y = sliderRect.y - 5;
        labelRect.w = surface->w;
        labelRect.h = surface->h;

        SDL_FreeSurface(surface);
    }
}