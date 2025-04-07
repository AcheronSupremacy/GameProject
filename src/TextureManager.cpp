//
// Created by ACER on 4/1/2025.
//

#include "TextureManager.hpp"
#include <iostream>
#include <SDL_image.h>

std::map<std::string, SDL_Texture*> TextureManager::textures;

SDL_Texture* TextureManager::LoadTexture(const std::string& path, SDL_Renderer* renderer) {
    if (textures.find(path) != textures.end()) {
        return textures[path];
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create texture: " << path << std::endl;
        return nullptr;
    }

    textures[path] = texture;
    return texture;
}

void TextureManager::Draw(SDL_Renderer* renderer, SDL_Texture* texture,
                         const SDL_Rect& srcRect, const SDL_Rect& destRect) {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void TextureManager::Clean() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
}