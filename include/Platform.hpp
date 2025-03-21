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
    void render(SDL_Renderer* renderer, const SDL_Rect& camera) const {
        SDL_Rect renderRect = {rect.x - camera.x, rect.y - camera.y, rect.w, rect.h};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &renderRect);
    }
};
#endif //PLATFORM_HPP
