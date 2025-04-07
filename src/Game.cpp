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
    background->addLayer("assets/background/background_layer_2.png",0.15f);
    background->addLayer("assets/background/background_layer_3.png",0.3f);
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
    camera.x = player->rect.x + (player->rect.w / 2) - (windowWidth / 2);
    camera.y = player->rect.y + (player->rect.h / 2) - (windowHeight / 2);

    camera.x = std::clamp(camera.x, 0, LEVEL_WIDTH - windowWidth);
    camera.y = std::clamp(camera.y, 0, LEVEL_HEIGHT - windowHeight);
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
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);
    static Uint32 lastTime = SDL_GetTicks();
    static bool spaceWasReleased = true;
    static bool shiftWasReleased = true;
    float deltaTime = calculateDeltaTime();

    if (keystates[SDL_SCANCODE_A]) player->moveLeft();
    else if (keystates[SDL_SCANCODE_D]) player->moveRight();
    else player->stopMoving();


    if (keystates[SDL_SCANCODE_SPACE]) {
        if (spaceWasReleased) {
            player->jump();
            spaceWasReleased = false;
        }
    } else spaceWasReleased = true;
    if (keystates[SDL_SCANCODE_LSHIFT]) {
        if (shiftWasReleased) {
            player->dash();
            shiftWasReleased = false;
        }
    } else shiftWasReleased = true;
    float previousX = player->rect.x;
    player->update(deltaTime, platforms);
    float deltaX = player->rect.x - previousX;
    background->update(deltaX);
    centerCameraOnPlayer();
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
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    delete player;
}
