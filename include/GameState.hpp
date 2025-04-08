//
// Created by ACER on 4/6/2025.
//

#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

class Game;

class GameState {
public:
    GameState() = default;
    virtual ~GameState() = default;

    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void handleEvents(SDL_Event& e) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    void setGame(Game* gamePtr) { game = gamePtr; }
protected:
    Game* game = nullptr;
};

class GameStateManager {
public:
    GameStateManager(Game* gamePtr) : game(gamePtr) {}
    ~GameStateManager() = default;

    template<typename T>
    void registerState(const std::string& name) {
        states[name] = [this] {
            auto state = std::make_unique<T>();
            state->setGame(game);
            return state;
        };
    }

    void changeState(const std::string& name) {
        if (currentState) {
            currentState->exit();
        }

        auto it = states.find(name);
        if (it != states.end()) {
            currentState = it->second();
            currentStateName = name;
            currentState->enter();
        }
    }

    const std::string& getCurrentStateName() const { return currentStateName; }

    void handleEvents(SDL_Event& e) {
        if (currentState) currentState->handleEvents(e);
    }

    void update(float deltaTime) {
        if (currentState) currentState->update(deltaTime);
    }

    void render(SDL_Renderer* renderer) {
        if (currentState) currentState->render(renderer);
    }

private:
    Game* game = nullptr;
    std::unique_ptr<GameState> currentState = nullptr;
    std::string currentStateName = "";
    std::unordered_map<std::string, std::function<std::unique_ptr<GameState>()>> states;
};
#endif //GAMESTATE_HPP
