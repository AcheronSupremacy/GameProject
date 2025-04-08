//
// Created by ACER on 4/8/2025.
//
#include "Slider.hpp"
#include <iostream>

Slider::Slider(int x, int y, int width, int height,
               const std::string& labelText,
               SDL_Color sliderColor, SDL_Color handleColor)
    : sliderRect{x, y, width, height},
      handleRect{x, y - height/2, height*2, height*2},
      label(labelText),
      font(TTF_OpenFont("assets/PixelifySans-Regular.ttf", 24)),
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

    if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT && isDragging) {
        isDragging = false;
        return true;
    }

    int mouseX = 0, mouseY = 0;
    
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        mouseX = e.button.x;
        mouseY = e.button.y;
    } 
    else if (e.type == SDL_MOUSEMOTION) {
        mouseX = e.motion.x;
        mouseY = e.motion.y;

        if (!isDragging) {
            return false;
        }
    }
    else {
        return false;
    }

    SDL_Point mousePoint = {mouseX, mouseY};

    bool mouseInsideSlider = SDL_PointInRect(&mousePoint, &sliderRect);
    bool mouseInsideHandle = SDL_PointInRect(&mousePoint, &handleRect);

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (!(mouseInsideHandle || mouseInsideSlider)) {
            return false;
        }
        
        isDragging = true;

        int newValue = 100 * (mouseX - sliderRect.x) / sliderRect.w;
        newValue = std::max(0, std::min(100, newValue)); // Constrain to valid range
        
        if (newValue != currentValue) {
            currentValue = newValue;
            updateHandlePosition();
            valueChanged = true;
        }
    }
    else if (e.type == SDL_MOUSEMOTION && isDragging) {
        int constrainedX = std::max(sliderRect.x, std::min(sliderRect.x + sliderRect.w, mouseX));
        int newValue = 100 * (constrainedX - sliderRect.x) / sliderRect.w;
        newValue = std::max(0, std::min(100, newValue)); // Constrain to valid range
        
        if (newValue != currentValue) {
            currentValue = newValue;
            updateHandlePosition();
            valueChanged = true;
        }
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

void Slider::stopDragging() {
    isDragging = false;
}
