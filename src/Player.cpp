//
// Created by ACER on 3/12/2025.
//
#include "Player.hpp"
#include <algorithm>

Player::Player(int x, int y) :
    rect{x, y, 32, 32},
    velocityX(0),
    velocityY(0),
    isGrounded(false),
    gravity(1800.0f),
    jumpForce(-650.0f),
    moveSpeed(400.0f),
    airResistance(0.8f),
    groundFriction(0.7f),
    terminalVelocity(1200.0f) {}

void Player::update(float deltaTime, const std::vector<Platform>& platforms) {
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

        if (!isWallSliding) {
            velocityY = std::min(velocityY + gravity * deltaTime, terminalVelocity);
        }
    }

    SDL_Rect prevRect = rect;
    rect.x += static_cast<int>(velocityX * deltaTime);
    rect.y += static_cast<int>(velocityY * deltaTime);

    handleCollisions(platforms);
    handleWallSlide(deltaTime);

    isGrounded = (rect.y == prevRect.y && velocityY >= 0);
}

void Player::dash() {
    if (canDash && !isGrounded) {
        isDashing = true;
        canDash = false;
        dashTimer = dashDuration;

        if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_A]) {
            velocityX = -dashSpeed;
        }
        else if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_D]) {
            velocityX = dashSpeed;
        }
        else {
            velocityX = dashSpeed;
        }

        velocityY = 0;
    }
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

void Player::handleCollisions(const std::vector<Platform>& platforms) {
    for (const auto& platform : platforms) {
        SDL_Rect intersect;
        if (SDL_IntersectRect(&rect, &platform.rect, &intersect)) {
            bool horizontalCollision = (intersect.w < intersect.h);
            bool fromTop = (rect.y + rect.h - intersect.h <= platform.rect.y);
            bool fromLeft = (rect.x + rect.w - intersect.w <= platform.rect.x);

            if (horizontalCollision) {
                if (fromLeft) {
                    rect.x -= intersect.w;
                } else {
                    rect.x += intersect.w;
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
                if (horizontalCollision) {
                    if (fromLeft) {
                        onRightWall = true;
                    } else {
                        onLeftWall = true;
                    }
            }
        }
    }
}
