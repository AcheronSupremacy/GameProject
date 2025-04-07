//
// Created by ACER on 4/7/2025.
//
// src/Background.cpp
#include "Background.hpp"
#include "Player.hpp"
#include <iostream>
#include <SDL_image.h>

Background::Background(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight) {}
Background::~Background() {
    for (auto& layer : layers) {
        SDL_DestroyTexture(layer.texture);
    }
}

bool Background::addLayer(const std::string& imagePath, float scrollSpeed) {
    Layer layer;
    layer.scrollSpeed = scrollSpeed;
    layer.position = 0.0f;

    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) return false;
    layer.texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!layer.texture) return false;
    layer.width = surface->w;
    layer.height = surface->h;

    SDL_FreeSurface(surface);

    layers.push_back(layer);
    return true;
}

void Background::update(float playerDeltaX) {
    for (auto& layer : layers) {
        float movement = playerDeltaX * layer.scrollSpeed;
        layer.position += movement;

        if (layer.position > layer.width) {
            layer.position -= layer.width;
        } else if (layer.position < 0) {
            layer.position += layer.width;
        }
    }
}

void Background::render() { //infinite scrolling background
    for (auto& layer : layers) {
        int posX = static_cast<int>(layer.position) % layer.width;
        if (posX < 0) posX += layer.width;

        SDL_Rect srcRect1 = { posX, 0, layer.width - posX, layer.height };

        SDL_Rect dstRect1 = {
            0,
            0,
            static_cast<int>((float)srcRect1.w / layer.height * screenHeight),
            screenHeight
        };
        SDL_Rect srcRect2 = { 0, 0, posX, layer.height };
        SDL_Rect dstRect2 = {
            dstRect1.w,
            0,
            static_cast<int>((float)srcRect2.w / layer.height * screenHeight),
            screenHeight
        };

        SDL_RenderCopy(renderer, layer.texture, &srcRect1, &dstRect1);
        if (posX > 0) {
            SDL_RenderCopy(renderer, layer.texture, &srcRect2, &dstRect2);
        }
    }
}