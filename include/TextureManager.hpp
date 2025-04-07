//
// Created by ACER on 3/27/2025.
//

#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#pragma once
#include <SDL.h>
#include <string>
#include <map>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer);
    static void Draw(SDL_Renderer* renderer, SDL_Texture* texture,
                    const SDL_Rect& srcRect, const SDL_Rect& destRect);
    static void Clean();

private:
    static std::map<std::string, SDL_Texture*> textures;
};
#endif //TEXTUREMANAGER_HPP
