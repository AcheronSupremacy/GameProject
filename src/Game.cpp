//
// Created by ACER on 2/19/2025.
//
#include <algorithm>
#include "Game.hpp"
#include "SDL2/SDL.h"
#include <iostream>
#include <chrono>
#include <SDL_image.h>
#include "SDL_ttf.h"
#include "TextureManager.hpp"

Game::Game() {
    this->is_running = false;
    this->window = nullptr;
    this->renderer = nullptr;
    this->player = nullptr;
}

Game::~Game() = default;

void Game::init(const std::string &title, const int w, const int h) {
    windowWidth = w;
    windowHeight = h;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
        this->renderer = SDL_CreateRenderer(this->window, -1, 0);
        this->is_running = true;
    } else {
        this->is_running = false;
    }
    int imgFlags = IMG_INIT_PNG;
    if (!IMG_Init(imgFlags)) {
        std::cerr << "SDL_image failed: " << IMG_GetError() << std::endl;
    }
    background = new Background(renderer, windowWidth, windowHeight);
    background->addLayer("assets/background/background_layer_1.png",0.05f);
    background->addLayer("assets/background/background_layer_2.png",0.1f);
    background->addLayer("assets/background/background_layer_3.png",0.2f);
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("assets/PixelifySans-Regular.ttf", 20);

    player = new Player(100, 100, 32, 32);

    loadLevel();

    camera = {0, 0, windowWidth, windowHeight};
}

float Game::calculateDeltaTime() {
    using Clock = std::chrono::high_resolution_clock;
    static auto lastTime = Clock::now();

    auto currentTime = Clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;

    return std::min(deltaTime, 0.1f);
}
void Game::loadLevel() {
    platforms.emplace_back(0, LEVEL_HEIGHT - 50, LEVEL_WIDTH, 50);

    platforms.emplace_back(300, 600, 200, 20);
    platforms.emplace_back(600, 500, 200, 20);
    platforms.emplace_back(900, 400, 200, 20);
    platforms.emplace_back(1200, 300, 200, 20);
}

void Game::centerCameraOnPlayer() {
    int targetX = player->rect.x - windowWidth / 2;

    if (player->getIsDashing()) {
        camera.x = camera.x + (targetX - camera.x) * cameraSmoothing;
    } else {
        camera.x = targetX;
    }

    if (camera.x < 0) {
        camera.x = 0;
    } else if (camera.x > LEVEL_WIDTH - camera.w) {
        camera.x = LEVEL_WIDTH - camera.w;
    }

    camera.y = 0;
}

void Game::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
            break;
            default:
                break;

        }
    }
}

void Game::update() {
    float deltaTime = calculateDeltaTime();
    player->handleInput();
    player->update(deltaTime, platforms);

    centerCameraOnPlayer();

    float cameraDeltaX = camera.x - previousCameraX;
    previousCameraX = camera.x;
    background->update(cameraDeltaX);
}

void Game::render() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
    background->render();
    for (auto& platform : platforms) {
        platform.render(renderer, camera);
    }

    player->render(renderer, camera);

    SDL_RenderPresent(this->renderer);
}

void Game::clean() const {
    if (background) {
        delete background;
    }
    TextureManager::Clean();
    delete player;
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}
