#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "app.hpp"

int main(int argc, char* argv[]) {

    struct Params {
        int winWidth = 640;
        int winHeight = 360;
        int tileSize = 40;
    };

    Params par;
    par.winWidth = 1280;
    par.winHeight = 720;
    par.tileSize = 20;

    App app;
    app.init(par.winWidth, par.winHeight);
    app.initTiles(par.tileSize);

    bool running = true;
    bool showPattern = false; // Added to persist the pattern on screen
    SDL_Event e;

    const float deltaTime = 0.01f;
    float accumulator = 0.0f;
    float currentTime = SDL_GetTicks() * 0.001f;

    while (running) {

        int startTicks = SDL_GetTicks();
        float newTime = SDL_GetTicks() * 0.001f;
        float frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= deltaTime) {
            while (SDL_PollEvent(&e)) {

                switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_RETURN) {
                        app.clear();
                        app.mapTiles();
                        showPattern = true; // Trigger persistence
                    }
                }

            }
            accumulator -= deltaTime;
        }
        const float alpha = accumulator / deltaTime;

        // Rendering logic moved here to run every frame
        if (showPattern) {
            app.showTiles();
        }

        app.display();

        int targetTicks = 1000 / app.getRefreshRate();
        int frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < targetTicks) {
            SDL_Delay(targetTicks - frameTicks);
        }
    }

    return app.close();
}
