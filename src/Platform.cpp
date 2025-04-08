#include "Platform.hpp"
#include "TextureManager.hpp"

Platform::Platform(int x, int y, int w, int h, SDL_Texture* texture) 
    : rect{x, y, w, h}, texture(texture) {

    leftEdgeTile = {120, 0, TILE_SIZE, TILE_SIZE};
    middleTile = {120 + 24, 0, TILE_SIZE, TILE_SIZE};
    rightEdgeTile = {215 - 24, 0, TILE_SIZE, TILE_SIZE};

    bottomHalfTile = {120 + 24, TILE_SIZE/2, TILE_SIZE, TILE_SIZE/2};
}

void Platform::render(SDL_Renderer* renderer, const SDL_Rect& camera) {
    SDL_Rect destRect = {
        rect.x - camera.x,
        rect.y - camera.y,
        rect.w,
        rect.h
    };

    if (destRect.x + destRect.w < 0 || destRect.x > camera.w ||
        destRect.y + destRect.h < 0 || destRect.y > camera.h) {
        return;
    }
    
    if (texture) {
        int numTiles = rect.w / TILE_SIZE;
        if (rect.w % TILE_SIZE > 0) numTiles++;

        for (int i = 0; i < numTiles; i++) {
            SDL_Rect tileDestRect = {
                destRect.x + i * TILE_SIZE,
                destRect.y,
                TILE_SIZE,
                TILE_SIZE
            };

            SDL_Rect* srcRect;
            if (i == 0) {
                srcRect = &leftEdgeTile;
            } else if (i == numTiles - 1) {
                srcRect = &rightEdgeTile;
            } else {
                srcRect = &middleTile;
            }

            SDL_RenderCopy(renderer, texture, srcRect, &tileDestRect);

            if (rect.h > TILE_SIZE) {
                for (int j = 1; j < rect.h / TILE_SIZE; j++) {
                    SDL_Rect halfTileDestRect = {
                        tileDestRect.x,
                        tileDestRect.y + TILE_SIZE*j,
                        TILE_SIZE,
                        rect.h - TILE_SIZE*j
                    };

                    SDL_RenderCopy(renderer, texture, &bottomHalfTile, &halfTileDestRect);
                }
            }
        }
    } else {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // brown color
        SDL_RenderFillRect(renderer, &destRect);
    }
}

bool Platform::collidesWith(const SDL_Rect& other) const {
    return SDL_HasIntersection(&rect, &other);
}