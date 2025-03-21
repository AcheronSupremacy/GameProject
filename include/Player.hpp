//
// Created by ACER on 3/12/2025.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#pragma once
#include <SDL.h>
#include <vector>
#include "Platform.hpp"

class Player {
public:
    Player(int x, int y, int w, int h);
    void update(float deltaTime const std::vector<Platform>& platforms);
    void render(SDL_Renderer* renderer);
    void dash();
    void handleWallSlide(float deltaTime);

    SDL_Rect rect;

    bool isDashing = false;
    bool isWallSliding = false;
    bool canDash = true;
    bool onLeftWall = false;
    bool onRightWall = false;
private:
    void handleCollisions(const std::vector<Platform>& platforms);

    float velocityX;
    float velocityY;
    bool isGrounded;
    float gravity;
    float jumpForce;
    float moveSpeed;
    float airResistance;
    float groundFriction;
    float terminalVelocity;

    float dashSpeed = 1200.0f;
    float dashDuration = 0.15f;
    float dashCooldown = 1.0f;
    float dashTimer = 0.0f;

    float wallSlideSpeed = 150.0f;
    float wallJumpForce = 500.0f;
    float wallJumpHorizontalForce = 300.0f;
};

#endif