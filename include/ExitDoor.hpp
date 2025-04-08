//
// Created by ACER on 4/8/2025.
//

#ifndef EXITDOOR_HPP
#define EXITDOOR_HPP
#pragma once
#include <SDL.h>
#include <vector>

class ExitDoor {
public:
    ExitDoor(int x, int y, int w, int h);
    ~ExitDoor();

    void render(SDL_Renderer* renderer, const SDL_Rect& camera) const;
    void update(float deltaTime);
    SDL_Rect getRect() const { return rect; }

    bool loadTexture(SDL_Renderer* renderer);

private:
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;

    int frameWidth = 64;
    int frameHeight = 64;
    int frameCount = 8;
    int currentFrame = 0;
    float frameTime = 0;
    const float frameDuration = 0.2f;

    std::vector<SDL_Rect> frames;
};
#endif //EXITDOOR_HPP
