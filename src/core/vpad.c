// A virtual gamepad
// (c) 2019 Jani Nykänen

#include "vpad.h"

#include "../util/mathext.h"


// Create a virtual gamepad
Vpad createVpad() {

    Vpad p;
    uint8 i;

    // Reset buttons
    for(i = 0; i < VPAD_BUTTONS; ++ i) {

        p.buttons[i].key = -1;
        p.buttons[i].state = Up;
    }

    // Reset stick
    p.stick = vec2(0, 0);
    p.oldStick = vec2(0, 0);
    p.delta = vec2(0, 0);

    return p;
}


// Add a button
void vpadAddButton(Vpad* vpad, uint8 index, int16 key) {

    if(index >= VPAD_BUTTONS)
        return;

    vpad->buttons[index].state = Up;
    vpad->buttons[index].key = key;
}


// Update button states
void vpadUpdate(Vpad* vpad, InputManager* input) {

    uint8 i;

    // Update buttons
    for(i = 0; i < VPAD_BUTTONS; ++ i) {

        if(vpad->buttons[i].key < 0)
            continue;

        vpad->buttons[i].state =
            inputGetKey(input, (uint8)vpad->buttons[i].key);
    }

    // Update stick
    vpad->stick.x = 0;
    vpad->stick.y = 0;
    // Horizontal
    if(inputGetArrow(input, ArrowLeft) == Down) {

        vpad->stick.x = -128;
    }
    else if(inputGetArrow(input, ArrowRight) == Down) {

        vpad->stick.x = 128;
    }

    // Vertical
    if(inputGetArrow(input, ArrowUp) == Down) {

        vpad->stick.y = -128;
    }
    else if(inputGetArrow(input, ArrowDown) == Down) {

        vpad->stick.y = 128;
    }

    // Make sure the values are within B(0,1)
    if(vpad->stick.x != 0 &&
       extAbs(vpad->stick.x) == extAbs(vpad->stick.y)) {

        vpad->stick.x = vpad->stick.x < 0 ? -91 : 91;
        vpad->stick.y = vpad->stick.y < 0 ? -91 : 91;
    }

    // Compute delta
    vpad->delta.x = vpad->stick.x - vpad->oldStick.x;
    vpad->delta.y = vpad->stick.y - vpad->oldStick.y;
    vpad->oldStick = vpad->stick;
}
