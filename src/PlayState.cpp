//
// Created by ACER on 4/8/2025.
//
// src/PlayState.cpp
#include "PlayState.hpp"
#include "Game.hpp"
#include <iostream>

PlayState::PlayState() : exitDoor(nullptr) {}


void PlayState::enter() {
    if (!player) {
        player = new Player(100, 100, 32, 32);
    }

    if (!background) {
        background = new Background(game->getRenderer(), game->getWindowWidth(), game->getWindowHeight());
        background->addLayer("assets/background/background_layer_1.png", 0.05f);
        background->addLayer("assets/background/background_layer_2.png", 0.1f);
        background->addLayer("assets/background/background_layer_3.png", 0.2f);
    }

    if (platforms.empty()) {
        camera = {0, 0, game->getWindowWidth(), game->getWindowHeight()};
        previousCameraX = 0;

        loadLevel();
    }
    if (!exitDoor) {
        exitDoor = new ExitDoor(LEVEL_WIDTH - 150, LEVEL_HEIGHT - 150, 50, 100);
    }
}

void PlayState::exit() {
    if (game->getStateManager()->getCurrentStateName() == "menu") {
        if (player) {
            delete player;
            player = nullptr;
        }

        if (background) {
            delete background;
            background = nullptr;
        }
        if (exitDoor) {
            delete exitDoor;
            exitDoor = nullptr;
        }
        platforms.clear();
    }
}

void PlayState::loadLevel() {
    platforms.emplace_back(0, LEVEL_HEIGHT - 50, LEVEL_WIDTH, 50);
    platforms.emplace_back(300, 600, 200, 20);
    platforms.emplace_back(600, 500, 200, 20);
    platforms.emplace_back(900, 400, 200, 20);
    platforms.emplace_back(1200, 300, 200, 20);
    platforms.emplace_back(1500, 450, 200, 20);
    platforms.emplace_back(1800, 350, 200, 20);
    platforms.emplace_back(2100, 250, 200, 20);
    platforms.emplace_back(LEVEL_WIDTH - 200, LEVEL_HEIGHT - 150, 200, 20);
}

void PlayState::centerCameraOnPlayer() {
    int targetX = player->rect.x - game->getWindowWidth() / 2;

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
void PlayState::checkGameOver() {
    if (player->rect.y > LEVEL_HEIGHT + gameOverThreshold) {
        game->getStateManager()->changeState("gameover");
    }
}

void PlayState::checkLevelComplete() {
    if (exitDoor) {
        SDL_Rect playerRect = player->rect;
        SDL_Rect doorRect = exitDoor->getRect();

        if (SDL_HasIntersection(&playerRect, &doorRect)) {
            game->getStateManager()->changeState("levelcomplete");
        }
    }
}
void PlayState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        game->getStateManager()->changeState("pause");
    }
}

void PlayState::update(float deltaTime) {
    player->handleInput();
    player->update(deltaTime, platforms);

    centerCameraOnPlayer();

    float cameraDeltaX = camera.x - previousCameraX;
    previousCameraX = camera.x;
    background->update(cameraDeltaX);
    checkGameOver();
    checkLevelComplete();
}

void PlayState::render(SDL_Renderer* renderer) {
    background->render();

    for (auto& platform : platforms) {
        platform.render(renderer, camera);
    }
    if (exitDoor) {
        exitDoor->render(renderer, camera);
    }

    player->render(renderer, camera);
}