// Input manager
// (c) 2019 Jani Nykänen

#ifndef __INPUT__
#define __INPUT__

#include "types.h"

#include <dos.h>
#include <conio.h>
#include <bios.h>

#define KEY_BUFFER_SIZE 0x60

// Arrow key enumerations
enum {

    ArrowRight = 0,
    ArrowUp = 1,
    ArrowLeft = 2,
    ArrowDown = 3,
};

// Button states
enum {
    Up = 0,
    Down = 1,
    Pressed = 2,
    Released = 3,
};

// Input manager
typedef struct {

    // Normal keys
    uint8 normalKeys[KEY_BUFFER_SIZE];
    // Extended keys
    uint8 extKeys[KEY_BUFFER_SIZE];

    // Old key states (normal)
    uint8 oldNormals[KEY_BUFFER_SIZE];
    // Old key states (extended)
    uint8 oldExt[KEY_BUFFER_SIZE];
    // Read states (normal)
    bool normalRead[KEY_BUFFER_SIZE];
    // Read states (extended)
    bool extRead[KEY_BUFFER_SIZE];

    // Handlers
    void far interrupt (*oldHandler)(void);

} InputManager;


// Create input manager
InputManager* createInputManager();

// Destroy input manager
void destroyInputManager(InputManager* input);

// Get button
uint8 getButtonState(InputManager* input, uint8 button);

// Get arrow key state
uint8 getArrowKeyState(InputManager* input, uint8 d);

#endif // __INPUT__
