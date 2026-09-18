#include <stdio.h>
#include <SDL.h>
#include "core.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define CONFIG_IMPLEMENTATION
#include "app_config.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

int main(int argc, const char** argv) {
    AppConfig_t app_cfg = {0};
    cfg_error_t err = {0};
    if (AppConfig_load(&app_cfg, NULL, argc, argv, &err) != 0) return 1;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    printf("Creating window: %dx%d\n", (int)app_cfg.Window.width, (int)app_cfg.Window.height);
    
    SDL_Window *window = SDL_CreateWindow(
	    app_cfg.Window.title,
	    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	    app_cfg.Window.width, app_cfg.Window.height,
	    SDL_WINDOW_SHOWN
	);
	
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    
    int quit = 0;
    SDL_Event e;
    
    // Main application loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        
        // Render
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Dark grey background
            SDL_RenderClear(renderer);
            
            // TODO: Perlin noise texture rendering will go here
            
            SDL_RenderPresent(renderer); // VSync automatically caps framerate (no manual delays needed)
        }
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    AppConfig_free(&app_cfg);
    return 0;
}
