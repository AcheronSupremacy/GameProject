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
#include "MenuState.hpp"
#include "PlayState.hpp"
#include "PauseState.hpp"
#include "InstructionState.hpp"
#include "GameOverState.hpp"
#include "LevelCompleteState.hpp"

Game::Game() {
    this->is_running = false;
    this->window = nullptr;
    this->renderer = nullptr;
}

Game::~Game() = default;

void Game::init(const std::string &title, const int w, const int h) {
    windowWidth = w;
    windowHeight = h;
#ifndef NDEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
#endif
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
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf failed: " << TTF_GetError() << std::endl;
    }

    TTF_Init();
    stateManager = std::make_unique<GameStateManager>(this);
    stateManager->registerState<MenuState>("menu");
    stateManager->registerState<PlayState>("play");
    stateManager->registerState<PauseState>("pause");
    stateManager->registerState<InstructionState>("instructions");
    stateManager->registerState<GameOverState>("gameover" );
    stateManager->registerState<LevelCompleteState>("levelcomplete" );
    stateManager->pushState("menu");
}

float Game::calculateDeltaTime() {
    using Clock = std::chrono::high_resolution_clock;
    static auto lastTime = Clock::now();

    auto currentTime = Clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;

    return std::min(deltaTime, 0.1f);
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
        stateManager->handleEvents(event);
    }

}

void Game::update() {
    float deltaTime = calculateDeltaTime();
    stateManager->update(deltaTime);
}

void Game::render() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
    stateManager->render(renderer);

    SDL_RenderPresent(this->renderer);
}

void Game::clean() const {

    TextureManager::Clean();

    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}
