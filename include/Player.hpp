//
// Created by ACER on 3/12/2025.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Platform.hpp"

enum PlayerState {
    IDLE,
    RUNNING,
    JUMPING,
    FALLING,
    DASHING,
};

class Player {
public:
    Player(int x, int y, int w, int h);
    ~Player();
    
    void handleInput();
    void update(float deltaTime, const std::vector<Platform>& platforms);
    void render(SDL_Renderer* renderer, const SDL_Rect& camera);
    
    SDL_Rect rect;
    
    bool getIsDashing() const { return isDashing; }
    
private:
    float velocityX = 0;
    float velocityY = 0;
    const float gravity = 0.5f;
    const float jumpForce = -12.0f;
    const float moveSpeed = 5.0f;
    const float dashSpeed = 15.0f;
    const float dashDuration = 0.2f;
    const float dashCooldown = 0.5f;
    
    bool isOnGround = false;
    bool isDashing = false;
    float dashTimer = 0;
    float dashCooldownTimer = 0;

    SDL_Texture* spriteSheet1 = nullptr;
    SDL_Texture* spriteSheet2 = nullptr;
    PlayerState currentState = IDLE;
    int frameWidth = 56;
    int frameHeight = 56;

    std::vector<SDL_Rect> idleFrames;
    std::vector<SDL_Rect> runFrames;
    std::vector<SDL_Rect> jumpFrames;
    std::vector<SDL_Rect> fallFrames;
    std::vector<SDL_Rect> dashFrames;
    
    int currentFrame = 0;
    float frameTimer = 0;
    float frameDelay = 0.1f;
    
    void initializeAnimations();
    void updateAnimation(float deltaTime);
    PlayerState determineState();
    bool facingLeft = false;
};

#endif