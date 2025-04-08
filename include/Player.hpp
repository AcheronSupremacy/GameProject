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

    void update(float deltaTime, const std::vector<Platform>& platforms);
    void render(SDL_Renderer* renderer, const SDL_Rect& camera);
    void handleInput();

    [[nodiscard]] bool getIsGrounded() const { return isGrounded; }
    [[nodiscard]] bool getIsDashing() const { return isDashing; }
    SDL_Rect rect;



private:
    void handleCollisions(const std::vector<Platform>& platforms, const SDL_Rect& prevRect);
    void handleWallSlide();
    float velocityX;
    float velocityY;
    bool isGrounded;
    float gravity;
    float jumpForce;
    float moveSpeed;
    float airResistance;
    float groundFriction;
    float terminalVelocity;

    float jumpCooldown;
    float jumpTimer;



    float dashSpeed;
    float dashDuration;
    float dashCooldown;
    float dashTimer;
    bool isDashing;

    float wallSlideSpeed;
    float wallJumpForce;
    float wallJumpHorizontalForce;
    bool isWallSliding;
    bool canDash;
    bool onLeftWall;
    bool onRightWall;
};

#endif