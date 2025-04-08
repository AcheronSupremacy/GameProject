#include "SDL2/SDL.h"
#include "SDL2/SDL_main.h"
#include "Game.hpp"


Game* game;
constexpr int FPS = 60, min_frame_time = 1000 / FPS;
unsigned int frame_start, frame_time;

int main(int argc, char *argv[]) {
    game = new Game();
    game->init("Do you have skill issue?", 1280, 720);

    while (game->running()) {
        frame_start = SDL_GetTicks();
        game->handle_events();
        game->update();
        game->render();
        frame_time = SDL_GetTicks() - frame_start;

        if (frame_time < min_frame_time) {
            SDL_Delay(min_frame_time - frame_time);
        }
    }
    game->clean();
    delete game;
    game = nullptr;
    return 0;
}