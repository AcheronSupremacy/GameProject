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
    dashSpeed(5000.0f),
    dashDuration(0.2f),
    dashCooldown(0.5f),
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
        velocityY = 0;
        dashTimer -= deltaTime;
        if (dashTimer <= 0) {
            isDashing = false;
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

    if (isGrounded) {
        velocityY = 0;
    }

    SDL_LogVerbose(0, "Delta Time: %f", deltaTime);
    SDL_LogVerbose(0, "Velocity: %f %f", velocityX, velocityY);
    SDL_LogVerbose(0, "Terminal Velocity: %f", terminalVelocity);
    SDL_Rect prevRect = rect;
    rect.x += static_cast<int>(velocityX * deltaTime);
    rect.y += static_cast<int>(velocityY * deltaTime);
    handleCollisions(platforms, prevRect);
    handleWallSlide(deltaTime);


    rect.x = std::clamp(rect.x, 0, LEVEL_WIDTH - rect.w);
    rect.y = std::clamp(rect.y, 0, LEVEL_HEIGHT - rect.h);
}

void Player::handleInput() {
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);
    static bool spaceWasReleased = true;
    static bool shiftWasReleased = true;
    if (keystates[SDL_SCANCODE_A]) velocityX = -moveSpeed;
    else if (keystates[SDL_SCANCODE_D]) velocityX = moveSpeed;
    else velocityX = 0;

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
                dashTimer = dashDuration;
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
    SDL_LogVerbose(0, "Begin handling collisions");
    int collisionCount = 0;
    for (const auto& platform : platforms) {
        SDL_Rect intersect;
        if (SDL_IntersectRect(&rect, &platform.rect, &intersect)) {
            collisionCount++;
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
                // SDL_LogVerbose(0, "Vertical Collision Detected");
                // SDL_LogVerbose(0, "Intersect Rect: %d %d %d %d", intersect.x, intersect.y, intersect.w, intersect.h);
                isGrounded = true;
                if (fromTop) {
                    rect.y -= intersect.h - 1;
                    velocityY = 0;
                    isGrounded = true;
                } else {
                    rect.y += intersect.h;
                    velocityY = 0;
                }
            }
        }
    }
    if (collisionCount > 0) {
        SDL_LogVerbose(0, "Collision detected with %d platforms", collisionCount);
    } else {
        SDL_LogVerbose(0, "No collision detected");
        isGrounded = false;
    }
}

void Player::render(SDL_Renderer* renderer, const SDL_Rect& camera) {
    SDL_Rect renderRect = {rect.x - camera.x, rect.y - camera.y - 1, rect.w, rect.h};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &renderRect);
}
