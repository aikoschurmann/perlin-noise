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

#include <stdbool.h>

static bool load_application_config(AppConfig_t* out_config, int argc, const char** argv) {
    cfg_error_t err = {0};
    if (AppConfig_load(out_config, NULL, argc, argv, &err) != 0) {
        printf("Configuration failed to load.\n");
        return false;
    }
    return true;
}

static bool initialize_graphics(const AppConfig_t* config, SDL_Window** out_window, SDL_Renderer** out_renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    printf("Creating window: %dx%d\n", (int)config->window.width, (int)config->window.height);
    
    *out_window = SDL_CreateWindow(
        config->window.title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config->window.width, config->window.height,
        SDL_WINDOW_SHOWN
    );
    
    if (!*out_window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_RendererFlags render_flags = SDL_RENDERER_ACCELERATED;
    if (config->window.vsync) {
        render_flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    
    *out_renderer = SDL_CreateRenderer(*out_window, -1, render_flags);
    if (!*out_renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    return true;
}

static void render_frame(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Dark grey background
    SDL_RenderClear(renderer);
    
    // TODO: Perlin noise texture rendering will go here
    
    SDL_RenderPresent(renderer); 
}

#include "input.h"

static void run_main_loop(const AppConfig_t* config, SDL_Renderer* renderer) {
    int is_running = 1;
    SDL_Event event;
    
    Input_Initialize();
    
    while (is_running) {
        Uint32 frame_start = SDL_GetTicks();
        
        Input_Update(); // Cycle the key states for the new frame
        
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                is_running = 0;
            }
        }
        
        // Handle Input State (Universal)
        if (Input_IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
            is_running = 0;
        }
        
        render_frame(renderer);
        
        // If VSync is off, cap the framerate to target_fps
        if (!config->window.vsync && config->window.target_fps > 0) {
            Uint32 frame_time = SDL_GetTicks() - frame_start;
            Uint32 target_delay = 1000 / config->window.target_fps;
            
            if (frame_time < target_delay) {
                SDL_Delay(target_delay - frame_time);
            }
        }
    }
}

static void shutdown_application(AppConfig_t* config, SDL_Window* window, SDL_Renderer* renderer) {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    AppConfig_free(config);
}

int main(int argc, const char** argv) {
    AppConfig_t config = {0};
    if (!load_application_config(&config, argc, argv)) {
        return 1;
    }
    
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (!initialize_graphics(&config, &window, &renderer)) {
        shutdown_application(&config, window, renderer);
        return 1;
    }
    
    run_main_loop(&config, renderer);
    
    shutdown_application(&config, window, renderer);
    return 0;
}
