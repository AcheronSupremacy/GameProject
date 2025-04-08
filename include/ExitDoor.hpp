//
// Created by ACER on 4/8/2025.
//

#ifndef EXITDOOR_HPP
#define EXITDOOR_HPP
#pragma once
#include <SDL.h>

class ExitDoor {
public:
    ExitDoor(int x, int y, int w, int h);

    void render(SDL_Renderer* renderer, const SDL_Rect& camera) const;
    SDL_Rect getRect() const { return rect; }

private:
    SDL_Rect rect;
};
#endif //EXITDOOR_HPP
