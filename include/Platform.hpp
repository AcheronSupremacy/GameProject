//
// Created by ACER on 3/21/2025.
//

#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#pragma once
#include <SDL2/SDL.h>

class Platform {
public:
    Platform(int x, int y, int w, int h, SDL_Texture* texture = nullptr);
    
    void render(SDL_Renderer* renderer, const SDL_Rect& camera);
    bool collidesWith(const SDL_Rect& rect) const;
    
    SDL_Rect rect;
    
private:
    SDL_Texture* texture;

    static const int TILE_SIZE = 24;

    SDL_Rect leftEdgeTile;
    SDL_Rect middleTile;
    SDL_Rect rightEdgeTile;
    SDL_Rect bottomHalfTile;
};

#endif //PLATFORM_HPP
