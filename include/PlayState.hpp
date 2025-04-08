//
// Created by ACER on 4/8/2025.
//

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP
#pragma once
#include "GameState.hpp"
#include "Player.hpp"
#include "Background.hpp"
#include "Platform.hpp"
#include "ExitDoor.hpp"
#include <vector>

class PlayState : public GameState {
public:
    PlayState();

    void enter() override;
    void exit() override;
    void handleEvents(SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    Player* player = nullptr;
    Background* background = nullptr;
    std::vector<Platform> platforms;
    ExitDoor* exitDoor;
    SDL_Rect camera;
    float previousCameraX = 0;
    float cameraSmoothing = 0.5f;

    const int gameOverThreshold = 100;

    void loadLevel();
    void centerCameraOnPlayer();
    void checkGameOver();
    void checkLevelComplete();
};
#endif //PLAYSTATE_HPP
