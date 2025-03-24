//
// Created by ACER on 2/19/2025.
//

#ifndef GAME_HPP
#define GAME_HPP
#include "SDL2/SDL.h"
#include "string"
#include "Player.hpp"

class Game {
public:
    Game();
    ~Game();

    void init(const std::string &title, int w, int h);
    void handle_events();
    void update();
    void render() const;
    void clean() const;

    [[nodiscard]] bool running() const { return this->is_running; }
private:
    bool is_running;
    int windowWidth;
    int windowHeight;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Player* player;
    std::vector<Platform> platforms;

    SDL_Rect camera;

    void loadLevel();
    void centerCameraOnPlayer();
};
const int PLAYER_WIDTH = 32;
const int PLAYER_HEIGHT = 48;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 1440;
#endif //GAME_HPP
