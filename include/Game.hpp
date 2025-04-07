//
// Created by ACER on 2/19/2025.
//

#ifndef GAME_HPP
#define GAME_HPP
#include "Background.hpp"
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
    float calculateDeltaTime();
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
    float previousCameraX = 0;
    float cameraSmoothing = 0.1f;

    SDL_Texture* playerTexture;
    SDL_Texture* platformTexture;

    void loadLevel();
    void centerCameraOnPlayer();


    Background* background;
};
const int LEVEL_WIDTH = 2560;
const int LEVEL_HEIGHT = 720;
#endif //GAME_HPP
