#include "graphics.h"
#include <stdio.h>

bool Graphics_Initialize(GraphicsContext* ctx, const char* title, int width, int height, bool vsync) {
    ctx->width = width;
    ctx->height = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    ctx->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    if (!ctx->window) return false;

    SDL_RendererFlags render_flags = SDL_RENDERER_ACCELERATED;
    if (vsync) {
        render_flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    
    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, render_flags);
    if (!ctx->renderer) return false;
    
    ctx->noise_texture = SDL_CreateTexture(
        ctx->renderer, 
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );
    if (!ctx->noise_texture) return false;
    
    return true;
}

void Graphics_BeginFrame(GraphicsContext* ctx) {
    SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->renderer);
}

void* Graphics_LockTexture(GraphicsContext* ctx, int* out_pitch) {
    void* pixels = NULL;
    SDL_LockTexture(ctx->noise_texture, NULL, &pixels, out_pitch);
    return pixels;
}

void Graphics_UnlockTexture(GraphicsContext* ctx) {
    SDL_UnlockTexture(ctx->noise_texture);
}

void Graphics_DrawNoiseTexture(GraphicsContext* ctx) {
    SDL_RenderCopy(ctx->renderer, ctx->noise_texture, NULL, NULL);
}

void Graphics_EndFrame(GraphicsContext* ctx) {
    SDL_RenderPresent(ctx->renderer);
}

void Graphics_Shutdown(GraphicsContext* ctx) {
    if (ctx->noise_texture) SDL_DestroyTexture(ctx->noise_texture);
    if (ctx->renderer) SDL_DestroyRenderer(ctx->renderer);
    if (ctx->window) SDL_DestroyWindow(ctx->window);
    SDL_Quit();
}
