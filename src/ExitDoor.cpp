//
// Created by ACER on 4/8/2025.
//
#include "ExitDoor.hpp"

ExitDoor::ExitDoor(int x, int y, int w, int h) : rect{x, y, w, h} {}

void ExitDoor::render(SDL_Renderer* renderer, const SDL_Rect& camera) const {
    SDL_Rect renderRect = {rect.x - camera.x, rect.y - camera.y, rect.w, rect.h};

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Brown color
    SDL_RenderFillRect(renderer, &renderRect);

    SDL_Rect innerRect = {renderRect.x + 4, renderRect.y + 4, renderRect.w - 8, renderRect.h - 8};
    SDL_SetRenderDrawColor(renderer, 205, 133, 63, 255); // Light brown
    SDL_RenderFillRect(renderer, &innerRect);

    SDL_Rect knobRect = {renderRect.x + renderRect.w - 12, renderRect.y + renderRect.h/2 - 4, 8, 8};
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // Gold color
    SDL_RenderFillRect(renderer, &knobRect);
}