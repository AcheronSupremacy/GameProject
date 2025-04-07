// include/Background.h
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>


class Background {
private:
    struct Layer {
        SDL_Texture* texture;
        float scrollSpeed;
        int width;
        int height;
        float position;
    };

    std::vector<Layer> layers;
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;

public:
    Background(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~Background();

    bool addLayer(const std::string& imagePath, float scrollSpeed);
    void update(float playerDeltaX);
    void render();
};

#endif // BACKGROUND_H