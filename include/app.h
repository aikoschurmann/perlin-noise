#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <SDL.h>
#include "graphics.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "app_config.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

// ---------------------------------------------------------
// Application State
// ---------------------------------------------------------
typedef struct {
    AppConfig_t config;
    GraphicsContext graphics;
    bool is_running;
    
} Application;

bool App_Initialize(Application* app, int argc, const char** argv);
void App_Run(Application* app);
void App_Shutdown(Application* app);

#endif // APP_H
