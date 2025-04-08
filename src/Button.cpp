//
// Created by ACER on 4/8/2025.
//
// src/Button.cpp
#include "Button.hpp"
#include <iostream>

Button::Button(int x, int y, int w, int h,
             const std::string& text,
             SDL_Color bgColor,
             SDL_Color textColor)
    : rect{x, y, w, h}, text(text), bgNormal(bgColor), textColor(textColor) {

    bgHover = {
        static_cast<Uint8>(std::min(255, bgNormal.r + 30)),
        static_cast<Uint8>(std::min(255, bgNormal.g + 30)),
        static_cast<Uint8>(std::min(255, bgNormal.b + 30)),
        bgNormal.a
    };

    bgPressed = {
        static_cast<Uint8>(std::max(0, bgNormal.r - 30)),
        static_cast<Uint8>(std::max(0, bgNormal.g - 30)),
        static_cast<Uint8>(std::max(0, bgNormal.b - 30)),
        bgNormal.a
    };
}

bool Button::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        bool inside = (x >= rect.x && x <= rect.x + rect.w &&
                      y >= rect.y && y <= rect.y + rect.h);

        if (!inside) {
            currentState = NORMAL;
        } else {
            switch (e.type) {
                case SDL_MOUSEMOTION:
                    currentState = HOVERED;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    currentState = PRESSED;
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (currentState == PRESSED) {
                        if (onClick) {
                            onClick();
                        }
                    }
                    currentState = HOVERED;
                    break;
            }
        }

        return inside;
    }

    return false;
}

void Button::render(SDL_Renderer* renderer) {
    SDL_Color currentColor;
    switch (currentState) {
        case NORMAL: currentColor = bgNormal; break;
        case HOVERED: currentColor = bgHover; break;
        case PRESSED: currentColor = bgPressed; break;
    }

    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &rect);

    if (textTexture) {
        int textW, textH;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
        SDL_Rect textRect = {
            rect.x + (rect.w - textW) / 2,
            rect.y + (rect.h - textH) / 2,
            textW,
            textH
        };
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }
}

void Button::setText(const std::string& newText, SDL_Renderer* renderer, TTF_Font* font) {
    text = newText;

    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    if (font) {
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
        if (surface) {
            textTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            if (!textTexture) {
                std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
            }
        } else {
            std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        }
    } else {
        std::cerr << "Font is null when setting text: " << text << std::endl;
    }
}

void Button::setColors(SDL_Color normal, SDL_Color hover, SDL_Color pressed) {
    bgNormal = normal;
    bgHover = hover;
    bgPressed = pressed;
}

void Button::setCallback(std::function<void()> callback) {
    onClick = std::move(callback);
}

std::string Button::getText() const {
    return text;
}