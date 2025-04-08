#include "SDL2/SDL.h"
#include "SDL2/SDL_main.h"
#include "Game.hpp"


Game* game;
int main(int argc, char *argv[]) {
    game = new Game();
    game->init("Do you have skill issue?", 1280, 720);

    while (game->running()) {
        game->handle_events();
        game->update();
        game->render();
    }
    game->clean();
    delete game;
    game = nullptr;
    return 0;
}