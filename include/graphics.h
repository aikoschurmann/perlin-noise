#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* noise_texture;
    int width;
    int height;
} GraphicsContext;

// Initialize SDL, Window, Renderer, and the main streaming Texture
bool Graphics_Initialize(GraphicsContext* ctx, const char* title, int width, int height, bool vsync);

// Clear the screen to the background color
void Graphics_BeginFrame(GraphicsContext* ctx);

// Lock the texture to get a writeable pixel pointer for the CPU
void* Graphics_LockTexture(GraphicsContext* ctx, int* out_pitch);

// Unlock the texture and upload changes to the GPU
void Graphics_UnlockTexture(GraphicsContext* ctx);

// Draw the noise texture to the screen
void Graphics_DrawNoiseTexture(GraphicsContext* ctx);

// Present the frame to the screen
void Graphics_EndFrame(GraphicsContext* ctx);

// Clean up all resources
void Graphics_Shutdown(GraphicsContext* ctx);

#endif // GRAPHICS_H
