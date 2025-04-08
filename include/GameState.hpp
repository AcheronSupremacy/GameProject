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
    void registerState(const std::string& name, bool persistent = false) {
        stateFactories[name] = [this] {
            auto state = std::make_unique<T>();
            state->setGame(game);
            return state;
        };
        persistentStates.insert({name, persistent});
    }

    void changeState(const std::string& name) {
        if (currentState) {
            if (persistentStates[currentStateName]) {
                // For persistent states, store the current state if it's not already stored
                if (savedStates.find(currentStateName) == savedStates.end()) {
                    savedStates[currentStateName] = std::move(currentState);
                }
                currentState = nullptr;
            } else {
                // For non-persistent states, call exit and discard
                currentState->exit();
                currentState = nullptr;
            }
        }

        auto it = stateFactories.find(name);
        if (it != stateFactories.end()) {
            // Check if we have a saved instance of this state
            auto savedIt = savedStates.find(name);
            if (savedIt != savedStates.end()) {
                // Restore the saved state
                currentState = std::move(savedIt->second);
                savedStates.erase(savedIt);
            } else {
                // Create a new state
                currentState = it->second();
            }

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
    std::unordered_map<std::string, std::function<std::unique_ptr<GameState>()>> stateFactories;
    std::unordered_map<std::string, bool> persistentStates;
    std::unordered_map<std::string, std::unique_ptr<GameState>> savedStates;
};
#endif //GAMESTATE_HPP
