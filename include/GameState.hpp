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
#include <vector>

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
    explicit GameStateManager(Game* gamePtr) : game(gamePtr) {}
    ~GameStateManager() = default;

    template<typename T>
    void registerState(const std::string& name) {
        stateFactories[name] = [this] {
            auto state = std::make_unique<T>();
            state->setGame(game);
            return state;
        };
    }
    void pushState(const std::string& name) {
        auto it = stateFactories.find(name);
        if (it != stateFactories.end()) {
            if (!stateStack.empty() && name != "pause") {
                stateStack.back()->exit();
            }

            auto newState = it->second();
            newState->enter();

            stateStack.push_back(std::move(newState));
            stateNames.push_back(name);
        }
    }

    void popState() {
        if (!stateStack.empty()) {

            stateStack.back()->exit();
            stateStack.pop_back();
            stateNames.pop_back();

            if (!stateStack.empty()) {
                stateStack.back()->enter();
            }
        }
    }

    void changeState(const std::string& name) {
        if (!stateStack.empty()) {
            stateStack.back()->exit();
            stateStack.pop_back();
            stateNames.pop_back();
        }

        pushState(name);
    }
    void clearAndSetState(const std::string& name) {
        while (!stateStack.empty()) {
            SDL_LogVerbose(0, "Exiting state: %s", stateNames.back().c_str());
            stateStack.back()->exit();
            stateStack.pop_back();
            stateNames.pop_back();
        }

        pushState(name);
    }

    const std::string& getCurrentStateName() const {
        static std::string emptyString = "";
        return stateStack.empty() ? emptyString : stateNames.back();
    }

    void handleEvents(SDL_Event& e) {
        if (!stateStack.empty()) stateStack.back()->handleEvents(e);
    }

    void update(float deltaTime) {
        if (!stateStack.empty()) stateStack.back()->update(deltaTime);
    }

    void render(SDL_Renderer* renderer) {
        for (size_t i = 0; i < stateStack.size(); i++) {
            stateStack[i]->render(renderer);
        }
    }

private:
    Game* game = nullptr;
    std::vector<std::unique_ptr<GameState>> stateStack;
    std::vector<std::string> stateNames;
    std::unordered_map<std::string, std::function<std::unique_ptr<GameState>()>> stateFactories;
};
#endif //GAMESTATE_HPP
