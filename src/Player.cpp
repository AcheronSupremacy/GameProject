//
// Created by ACER on 3/12/2025.
//
#include "Player.hpp"
#include "TextureManager.hpp"
#include "AudioManager.hpp"
#include "Game.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>

Player::Player(int x, int y, int w, int h) : rect{x, y, w, h} {
    SDL_Renderer* renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
    
    spriteSheet1 = TextureManager::LoadTexture("assets/generic_char_v0.2/png/blue/char_blue_1.png", renderer);
    if (!spriteSheet1) {
        std::cerr << "Failed to load player sprite sheet 1!" << std::endl;
    }
    
    spriteSheet2 = TextureManager::LoadTexture("assets/generic_char_v0.2/png/blue/char_blue_2.png", renderer);
    if (!spriteSheet2) {
        std::cerr << "Failed to load player sprite sheet 2!" << std::endl;
    }

    initializeAnimations();
}

Player::~Player() {
}

void Player::initializeAnimations() {
    for (int i = 0; i < 6; i++) {
        idleFrames.push_back({i * frameWidth, 0, frameWidth, frameHeight});
    }

    for (int i = 0; i < 8; i++) {
        runFrames.push_back({i * frameWidth, frameHeight * 2, frameWidth, frameHeight});
    }

    for (int i = 0; i < 8; i++) {
        jumpFrames.push_back({i * frameWidth, 3 * frameHeight, frameWidth, frameHeight});
    }

    for (int i = 2; i < 8; i++) {
        fallFrames.push_back({i * frameWidth, 4 * frameHeight, frameWidth, frameHeight});
    }

    for (int i = 0; i < 8; i++) {
        dashFrames.push_back({i * frameWidth, 2 * frameHeight, frameWidth, frameHeight});
    }
}

void Player::handleInput() {
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    velocityX = 0;

    if (keystates[SDL_SCANCODE_A] || keystates[SDL_SCANCODE_LEFT]) {
        velocityX = -moveSpeed;
        facingLeft = true;
    }
    if (keystates[SDL_SCANCODE_D] || keystates[SDL_SCANCODE_RIGHT]) {
        velocityX = moveSpeed;
        facingLeft = false;
    }

    if ((keystates[SDL_SCANCODE_SPACE] || keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_UP]) && isOnGround) {
        velocityY = jumpForce;
        isOnGround = false;
        AudioManager::getInstance().playSoundEffect("jump");
    }

    if (keystates[SDL_SCANCODE_LSHIFT] && !isDashing && dashCooldownTimer <= 0) {
        isDashing = true;
        dashTimer = dashDuration;
        dashCooldownTimer = dashCooldown;
    }
}

void Player::update(float deltaTime, const std::vector<Platform>& platforms) {
    bool wasOnGround = isOnGround;

    if (!isOnGround) {
        velocityY += gravity;
    }

    if (isDashing) {
        dashTimer -= deltaTime;
        if (dashTimer <= 0) {
            isDashing = false;
        }
        else {
            velocityX = facingLeft ? -dashSpeed : dashSpeed;
        }
    }

    if (dashCooldownTimer > 0) {
        dashCooldownTimer -= deltaTime;
        if (dashCooldownTimer < 0) {
            dashCooldownTimer = 0;
        }
    }

    rect.x += velocityX;

    for (const auto& platform : platforms) {
        if (platform.collidesWith(rect)) {
            if (velocityX > 0) {
                rect.x = platform.rect.x - rect.w;
            } else if (velocityX < 0) {
                rect.x = platform.rect.x + platform.rect.w;
            }
            break;
        }
    }

    rect.x = std::clamp(rect.x, 0, LEVEL_WIDTH - rect.w);
    rect.y += velocityY;

    isOnGround = false;

    for (const auto& platform : platforms) {
        if (platform.collidesWith(rect)) {
            if (velocityY > 0) {
                rect.y = platform.rect.y - rect.h;
                velocityY = 0;
                isOnGround = true;
            } else if (velocityY < 0) {
                rect.y = platform.rect.y + platform.rect.h;
                velocityY = 0;
            }
            break;
        }
    }

    if (!isOnGround) {

        SDL_Rect extendedRect = rect;
        extendedRect.y += 2;
        
        for (const auto& platform : platforms) {
            if (platform.collidesWith(extendedRect)) {
                rect.y = platform.rect.y - rect.h;
                velocityY = 0;
                isOnGround = true;
                break;
            }
        }
    }

    if (isOnGround) {
        velocityY = 0;
    }

    updateAnimation(deltaTime);
}

PlayerState Player::determineState() {
    static PlayerState lastState = IDLE;
    static float stateChangeTimer = 0;
    
    PlayerState newState;
    
    if (isDashing) {
        newState = DASHING;
        stateChangeTimer = 0;
    } else if (!isOnGround) {
        newState = (velocityY < -0.1f) ? JUMPING : FALLING;
        stateChangeTimer = 0;
    } else if (abs(velocityX) > 0.1f) {
        newState = RUNNING;
        stateChangeTimer = 0;
    } else {
        newState = IDLE;
        if (lastState == FALLING) {
            stateChangeTimer += 0.016f;
            if (stateChangeTimer < 0.05f) {
                newState = lastState;
            }
        }
    }

    lastState = newState;
    return newState;
}

void Player::updateAnimation(float deltaTime) {
    frameTimer += deltaTime;

    if (frameTimer >= frameDelay) {
        frameTimer = 0;
        
        currentState = determineState();
        SDL_LogVerbose(0, "Update animation current State: %d", currentState);
        switch (currentState) {
            case IDLE:
                currentFrame = (currentFrame + 1) % idleFrames.size();
                break;
            case RUNNING:
                currentFrame = (currentFrame + 1) % runFrames.size();
                break;
            case JUMPING:
                currentFrame = (currentFrame + 1) % jumpFrames.size();
                break;
            case FALLING:
                currentFrame = (currentFrame + 1) % fallFrames.size();
                break;
            case DASHING:
                currentFrame = (currentFrame + 1) % dashFrames.size();
                break;
        }
        SDL_LogVerbose(0, "Update animation current frame: %d", currentFrame);
    }
}

void Player::render(SDL_Renderer* renderer, const SDL_Rect& camera) {
    if (!spriteSheet1 && !spriteSheet2) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        
        SDL_Rect destRect = {
            rect.x - camera.x,
            rect.y - camera.y,
            rect.w,
            rect.h
        };
        
        SDL_RenderFillRect(renderer, &destRect);
        return;
    }

    SDL_Texture* currentTexture = nullptr;
    SDL_Rect* currentSrcRect = nullptr;
    
    SDL_LogVerbose(0, "Render current State: %d", currentState);
    SDL_LogVerbose(0, "Render current frame: %d", currentFrame);
    switch (currentState) {
        case IDLE:
            currentTexture = spriteSheet1;
            currentSrcRect = &idleFrames[currentFrame];
            break;
        case RUNNING:
            currentTexture = spriteSheet1;
            currentSrcRect = &runFrames[currentFrame];
            break;
        case JUMPING:
            currentTexture = spriteSheet1;
            currentSrcRect = &jumpFrames[currentFrame];
            break;
        case FALLING:
            currentTexture = spriteSheet1;
            currentSrcRect = &fallFrames[currentFrame];
            break;
        case DASHING:
            currentTexture = spriteSheet2;
            currentSrcRect = &dashFrames[currentFrame];
            break;
    }

    SDL_Rect destRect = {
        rect.x - camera.x - (frameWidth - rect.w) / 2,
        rect.y - camera.y - (frameHeight - rect.h),
        frameWidth,
        frameHeight
    };

    SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    if (currentTexture && currentSrcRect) {
        SDL_RenderCopyEx(renderer, currentTexture, currentSrcRect, &destRect, 0, nullptr, flip);
    }
}
