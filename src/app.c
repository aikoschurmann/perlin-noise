#include "app.h"
#include "input.h"
#include <stdio.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define CONFIG_IMPLEMENTATION
#include "app_config.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

// ---------------------------------------------------------
// Application Pipeline
// ---------------------------------------------------------

bool App_Initialize(Application* app, int argc, const char** argv) {
    cfg_error_t err = {0};
    if (AppConfig_load(&app->config, NULL, argc, argv, &err) != 0) {
        printf("Configuration failed to load.\n");
        return false;
    }
    
    if (!Graphics_Initialize(
            &app->graphics, 
            app->config.window.title, 
            app->config.window.width, 
            app->config.window.height, 
            app->config.window.vsync)) {
        return false;
    }
    
    Input_Initialize();
    app->is_running = true;
    
    return true;
}

static void App_ProcessInput(Application* app) {
    SDL_Event event;
    Input_Update(); 
    
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            app->is_running = false;
        }
    }
    
    if (Input_IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        app->is_running = false;
    }
}

static void App_UpdateLogic(Application* app, float delta_time) {
    (void)app; 
    (void)delta_time;
}

static void App_Render(Application* app) {
    Graphics_BeginFrame(&app->graphics);

    int pitch; 
    void* pixels = Graphics_LockTexture(&app->graphics, &pitch);
    Uint32* pixel_array = (Uint32*)pixels;

    int width = app->config.window.width;
    int height = app->config.window.height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // TODO: Replace with calculate_perlin(x, y); 
            Uint8 color = (x ^ y) % 255; 
            pixel_array[y * (pitch / 4) + x] = (color << 24) | (color << 16) | (color << 8) | 255;
        }
    }
    
    Graphics_UnlockTexture(&app->graphics);
    Graphics_DrawNoiseTexture(&app->graphics);
    
    Graphics_EndFrame(&app->graphics);
}

void App_Run(Application* app) {
    Uint32 last_time = SDL_GetTicks();
    
    while (app->is_running) {
        Uint32 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        
        App_ProcessInput(app);
        App_UpdateLogic(app, delta_time);
        App_Render(app);
        
        if (!app->config.window.vsync && app->config.window.target_fps > 0) {
            Uint32 frame_time = SDL_GetTicks() - current_time;
            Uint32 target_delay = 1000 / app->config.window.target_fps;
            if (frame_time < target_delay) {
                SDL_Delay(target_delay - frame_time);
            }
        }
    }
}

void App_Shutdown(Application* app) {
    Graphics_Shutdown(&app->graphics);
    AppConfig_free(&app->config);
}
