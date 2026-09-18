#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <SDL.h>

void Input_Initialize(void);

// Call this ONCE per frame BEFORE your logic to snapshot the current keyboard & mouse state
void Input_Update(void); 

// --- Keyboard ---
bool Input_IsKeyDown(SDL_Scancode scancode);
bool Input_IsKeyPressed(SDL_Scancode scancode);  // Just pressed THIS frame
bool Input_IsKeyReleased(SDL_Scancode scancode); // Just released THIS frame

// --- Mouse ---
bool Input_IsMouseButtonDown(int button); // e.g., SDL_BUTTON_LEFT
bool Input_IsMouseButtonPressed(int button);
bool Input_IsMouseButtonReleased(int button);

int Input_GetMouseX(void);
int Input_GetMouseY(void);
int Input_GetMouseDeltaX(void);
int Input_GetMouseDeltaY(void);

#endif // INPUT_H
