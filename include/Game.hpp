//
// Created by ACER on 2/19/2025.
//

#ifndef GAME_HPP
#define GAME_HPP
#include "Background.hpp"
#include "SDL2/SDL.h"
#include "string"
#include "Player.hpp"
#include "GameState.hpp"
class GameStateManager;

class Game {
public:
    Game();
    ~Game();

    void init(const std::string &title, int w, int h);
    void handle_events();
    void update();
    void render() const;
    void clean() const;
    float calculateDeltaTime();
    [[nodiscard]] bool running() const { return this->is_running; }

    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    SDL_Renderer* getRenderer() const { return renderer; }
    GameStateManager* getStateManager() const { return stateManager.get(); }
private:
    bool is_running;
    int windowWidth;
    int windowHeight;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::unique_ptr<GameStateManager> stateManager;
};
const int LEVEL_WIDTH = 2560;
const int LEVEL_HEIGHT = 720;
#endif //GAME_HPP
