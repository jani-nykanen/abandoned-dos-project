// Input manager
// (c) 2019 Jani Nykänen

#ifndef __INPUT__
#define __INPUT__

#include "types.h"

#include <dos.h>
#include <conio.h>
#include <bios.h>

#define KEY_BUFFER_SIZE 0x60
#define MAX_BUTTON 7

// Button enumerations
enum {

    ArrowRight = 0,
    ArrowUp = 1,
    ArrowLeft = 2,
    ArrowDown = 3,
    ButtonFire1 = 4,
    ButtonFire2 = 5,
    ButtonStart = 6,
    ButtonCancel = 7,
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
uint8 inputGetButton(InputManager* input, uint8 button);

#endif // __INPUT__
