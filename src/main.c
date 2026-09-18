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
    printf("Hello from %s! Port %d\n", app_cfg.name, (int)app_cfg.port);
    printf("test3");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        printf("SDL2 initialized successfully!\n");
        SDL_Quit();
    }

    AppConfig_free(&app_cfg);
    return 0;
}
