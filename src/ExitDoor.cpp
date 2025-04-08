//
// Created by ACER on 4/8/2025.
//
#include "ExitDoor.hpp"
#include "TextureManager.hpp"
#include <SDL_image.h>

ExitDoor::ExitDoor(int x, int y, int w, int h) : rect{x, y, w, h}, texture(nullptr), frameCount(8), currentFrame(0), frameTime(0), frameDuration(0.1f) {
}

ExitDoor::~ExitDoor() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

bool ExitDoor::loadTexture(SDL_Renderer* renderer) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    
    texture = TextureManager::LoadTexture("assets/exit_door.png", renderer);
    
    if (!texture) {
        return false;
    }

    frames.clear();

    for (int i = 0; i < frameCount; ++i) {
        frames.push_back({i * frameWidth, 0, frameWidth, frameHeight});
    }
    
    return true;
}

void ExitDoor::update(float deltaTime) {
    frameTime += deltaTime;
    
    if (frameTime >= frameDuration) {
        frameTime = 0;
        currentFrame = (currentFrame + 1) % frameCount;
    }
    SDL_LogVerbose(0, "ExitDoor update current frame: %d (frameCount %d)", currentFrame, frameCount);
}

void ExitDoor::render(SDL_Renderer* renderer, const SDL_Rect& camera) const {
    SDL_Rect renderRect = {rect.x - camera.x, rect.y - camera.y, rect.w, rect.h};

    if (renderRect.x + renderRect.w < 0 || renderRect.x > camera.w ||
        renderRect.y + renderRect.h < 0 || renderRect.y > camera.h) {
        return;
    }
    
    if (texture && !frames.empty()) {
        SDL_LogVerbose(0, "ExitDoor render current frame: %d", currentFrame);
        SDL_RenderCopy(renderer, texture, &frames[currentFrame], &renderRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_RenderFillRect(renderer, &renderRect);

        SDL_Rect innerRect = {renderRect.x + 4, renderRect.y + 4, renderRect.w - 8, renderRect.h - 8};
        SDL_SetRenderDrawColor(renderer, 205, 133, 63, 255);
        SDL_RenderFillRect(renderer, &innerRect);

        SDL_Rect knobRect = {renderRect.x + renderRect.w - 12, renderRect.y + renderRect.h/2 - 4, 8, 8};
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderFillRect(renderer, &knobRect);
    }
}
