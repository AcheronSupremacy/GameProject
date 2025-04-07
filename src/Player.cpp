//
// Created by ACER on 3/12/2025.
//
#include "Player.hpp"
#include <algorithm>
#include "Game.hpp"

Player::Player(int x, int y, int w, int h) :
    rect{x, y, w, h},
    velocityX(0),
    velocityY(0),
    isGrounded(false),
    gravity(1800.0f),
    jumpForce(-650.0f),
    moveSpeed(400.0f),
    airResistance(0.8f),
    groundFriction(0.7f),
    terminalVelocity(1200.0f),
    jumpCooldown(0.3f),
    jumpTimer(0.0f),
    dashSpeed(2000.0f),
    dashDuration(0.2f),
    dashCooldown(0.2f),
    dashTimer(0.0f),
    isDashing(false),
    wallSlideSpeed(150.0f),
    wallJumpForce(500.0f),
    wallJumpHorizontalForce(300.0f),
    isWallSliding(false),
    canDash(true),
    onLeftWall(false),
    onRightWall(false) {}

void Player::update(float deltaTime, const std::vector<Platform>& platforms) {
    if (jumpTimer > 0.0f) jumpTimer -= deltaTime;
    if (isDashing) {
        dashTimer -= deltaTime;
        if (dashTimer <= 0) {
            isDashing = false;
            velocityY = 0;
            dashTimer = dashCooldown;
        }
    }
    else if (dashTimer > 0) {
        dashTimer -= deltaTime;
    }
    else {
        canDash = true;
    }

    if (!isDashing) {
        if (isGrounded) {
            velocityX *= groundFriction;
        } else {
            velocityX *= airResistance;
        }

        if (!isWallSliding&& !isDashing) {
            velocityY = std::min(velocityY + gravity * deltaTime, terminalVelocity);
        }
    }

    SDL_Rect prevRect = rect;
    rect.x += static_cast<int>(velocityX * deltaTime);
    rect.y += static_cast<int>(velocityY * deltaTime);
    handleCollisions(platforms, prevRect);
    handleWallSlide(deltaTime);


    rect.x = std::clamp(rect.x, 0, LEVEL_WIDTH - rect.w);
    rect.y = std::clamp(rect.y, 0, LEVEL_HEIGHT - rect.h);
    isGrounded = (rect.y == prevRect.y && velocityY >= 0);
}

void Player::handleInput() {
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);
    static bool spaceWasReleased = true;
    static bool shiftWasReleased = true;
    if (keystates[SDL_SCANCODE_A]) velocityX = -moveSpeed;
    else if (keystates[SDL_SCANCODE_D]) velocityX = moveSpeed;
    else velocityX = 0;;


    if (keystates[SDL_SCANCODE_SPACE]) {
        if (spaceWasReleased) {
            if (isGrounded) {
                velocityY = jumpForce;
                isGrounded = false;
                jumpTimer = jumpCooldown;
            }
            spaceWasReleased = false;
        }
    } else spaceWasReleased = true;
    if (keystates[SDL_SCANCODE_LSHIFT]) {
        if (shiftWasReleased) {
            if (canDash && dashTimer <= 0.0f) {
                isDashing = true;
                canDash = false;
                dashTimer = dashCooldown;
                velocityX = (velocityX < 0) ? -dashSpeed : dashSpeed;
                velocityY = 0;
            }
            shiftWasReleased = false;
        }
    } else shiftWasReleased = true;
}
void Player::handleWallSlide(float deltaTime) {
    if ((onLeftWall || onRightWall) && !isGrounded && velocityY > 0) {
        isWallSliding = true;
        velocityY = wallSlideSpeed;
        velocityX *= 0.9f;
    }
    else {
        isWallSliding = false;
    }

    if (isWallSliding && SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE]) {
        isWallSliding = false;
        velocityY = -wallJumpForce;
        velocityX = onLeftWall ? wallJumpHorizontalForce : -wallJumpHorizontalForce;
        canDash = true;
    }
}

void Player::handleCollisions(const std::vector<Platform>& platforms, const SDL_Rect& prevRect) {
    onLeftWall = false;
    onRightWall = false;

    for (const auto& platform : platforms) {
        SDL_Rect intersect;
        if (SDL_IntersectRect(&rect, &platform.rect, &intersect)) {
            bool horizontalCollision = (intersect.w < intersect.h);
            bool fromTop = (rect.y + rect.h - intersect.h <= platform.rect.y);

            if (horizontalCollision) {
                if (rect.x < platform.rect.x) {
                    rect.x -= intersect.w;
                    onRightWall = true;
                } else {
                    rect.x += intersect.w;
                    onLeftWall = true;
                }
                velocityX = 0;
            } else {
                if (fromTop) {
                    rect.y -= intersect.h;
                    velocityY = 0;
                    isGrounded = true;
                } else {
                    rect.y += intersect.h;
                    velocityY = 0;
                }
                    }
                }
            }
        }

void Player::render(SDL_Renderer* renderer, const SDL_Rect& camera) {
    SDL_Rect renderRect = {rect.x - camera.x, rect.y - camera.y, rect.w, rect.h};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &renderRect);
}
