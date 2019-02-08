// A virtual gamepad
// (c) 2019 Jani Nykänen

#ifndef __VPAD__
#define __VPAD__

#include "input.h"
#include "types.h"

// Button count
#define VPAD_BUTTONS 16

// Button type
typedef struct {

    int16 key;
    uint16 state;

} Button;

// Vpad type
typedef struct {
    
    // Buttons
    Button buttons[VPAD_BUTTONS];

    // "Analogue" stick
    Vector2 stick;
    // Old pos
    Vector2 oldStick;
    // Stick delta
    Vector2 delta;

} Vpad;


// Create a virtual gamepad
Vpad createVpad();

// Add a button
void vpadAddButton(Vpad* vpad, uint8 index, int16 key);

// Update
void vpadUpdate(Vpad* vpad, InputManager* input);

#endif // __VPAD__
