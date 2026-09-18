#include "input.h"
#include <string.h>

static Uint8 s_current_keys[SDL_NUM_SCANCODES];
static Uint8 s_previous_keys[SDL_NUM_SCANCODES];

static Uint32 s_current_mouse;
static Uint32 s_previous_mouse;
static int s_mouse_x, s_mouse_y;
static int s_mouse_dx, s_mouse_dy;
static int s_prev_mouse_x, s_prev_mouse_y;

void Input_Initialize(void) {
    memset(s_current_keys, 0, sizeof(s_current_keys));
    memset(s_previous_keys, 0, sizeof(s_previous_keys));
    s_current_mouse = 0;
    s_previous_mouse = 0;
    s_mouse_x = s_mouse_y = 0;
    s_mouse_dx = s_mouse_dy = 0;
    s_prev_mouse_x = s_prev_mouse_y = 0;
}

void Input_Update(void) {
    // Keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
    memcpy(s_previous_keys, s_current_keys, SDL_NUM_SCANCODES);
    memcpy(s_current_keys, state, SDL_NUM_SCANCODES);

    // Mouse
    s_previous_mouse = s_current_mouse;
    s_prev_mouse_x = s_mouse_x;
    s_prev_mouse_y = s_mouse_y;
    
    s_current_mouse = SDL_GetMouseState(&s_mouse_x, &s_mouse_y);
    s_mouse_dx = s_mouse_x - s_prev_mouse_x;
    s_mouse_dy = s_mouse_y - s_prev_mouse_y;
}

bool Input_IsKeyDown(SDL_Scancode scancode) {
    if (scancode < 0 || scancode >= SDL_NUM_SCANCODES) return false;
    return s_current_keys[scancode] != 0;
}

bool Input_IsKeyPressed(SDL_Scancode scancode) {
    if (scancode < 0 || scancode >= SDL_NUM_SCANCODES) return false;
    return s_current_keys[scancode] != 0 && s_previous_keys[scancode] == 0;
}

bool Input_IsKeyReleased(SDL_Scancode scancode) {
    if (scancode < 0 || scancode >= SDL_NUM_SCANCODES) return false;
    return s_current_keys[scancode] == 0 && s_previous_keys[scancode] != 0;
}

bool Input_IsMouseButtonDown(int button) {
    return (s_current_mouse & SDL_BUTTON(button)) != 0;
}

bool Input_IsMouseButtonPressed(int button) {
    return ((s_current_mouse & SDL_BUTTON(button)) != 0) && ((s_previous_mouse & SDL_BUTTON(button)) == 0);
}

bool Input_IsMouseButtonReleased(int button) {
    return ((s_current_mouse & SDL_BUTTON(button)) == 0) && ((s_previous_mouse & SDL_BUTTON(button)) != 0);
}

int Input_GetMouseX(void) { return s_mouse_x; }
int Input_GetMouseY(void) { return s_mouse_y; }
int Input_GetMouseDeltaX(void) { return s_mouse_dx; }
int Input_GetMouseDeltaY(void) { return s_mouse_dy; }
