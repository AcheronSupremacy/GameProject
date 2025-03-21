//
// Created by ACER on 3/21/2025.
//

#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#pragma once
#include <SDL.h>

struct Platform {
    SDL_Rect rect;
    Platform(int x, int y, int w, int h) : rect{x, y, w, h} {}
    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
};
#endif //PLATFORM_HPP
