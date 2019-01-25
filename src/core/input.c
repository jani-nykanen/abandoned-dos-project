// Input manager
// (c) 2019 Jani Nykänen

#include "input.h"

#include <stdlib.h>
#include <stdio.h>

// Button key codes
static const short BUTTON_KEY_CODES[] = {
    77, 72, 75, 80,
    44, 45, 28, 1,
};

// Reference to the current active
// input manager
static InputManager* inpRef;


// Keyboard interruption
// Source: some StackOverflow answer, I didn't
// copy the source when I found it, so I cannot
// say which one...
static void far interrupt handler() {

    static uint8 buffer;
    uint8 rawcode;
    uint8 makeBreak;
    int16 scancode;

    rawcode = inp(0x60); 
    makeBreak = !(rawcode & 0x80); 
    scancode = rawcode & 0x7F;

    if(buffer == 0xE0) { 

        if(scancode < 0x60) {

            inpRef->oldExt[scancode] = inpRef->extKeys[scancode];
            inpRef->extKeys[scancode] = makeBreak;
            
            if(inpRef->extKeys[scancode] != inpRef->oldExt[scancode])
                inpRef->extRead[scancode] = false;
        }
        buffer = 0;

    } 
    else if (buffer >= 0xE1 && buffer <= 0xE2) {

        buffer = 0;

    } 
    else if (rawcode >= 0xE0 && rawcode <= 0xE2) {

        buffer = rawcode; 

    } 
    else if (scancode < 0x60) {

        inpRef->oldNormals[scancode] = inpRef->normalKeys[scancode];
        inpRef->normalKeys[scancode] = makeBreak;

        if(inpRef->normalKeys[scancode] != inpRef->oldNormals[scancode])
            inpRef->normalRead[scancode] = false;
    }

    outp(0x20, 0x20);
}


// Get button value from an array
static char getValueFromArr(uint8* arr, bool* readArr, int16 id) {

    int16 s = BUTTON_KEY_CODES[id];
    uint8 state = arr[s];
    bool read = readArr[s];
    uint8 ret = 0;

    if(state == Down) {

        ret = read ? Down : Pressed;
    }
    else if(state == Up) {

        ret = read ? Up : Released;
    }
    readArr[s] = true;

    return ret;
}


// Create input manager
InputManager* createInputManager() {

    int16 i;

    // Allocate memory
    InputManager* input = (InputManager*)malloc(sizeof(InputManager));
    if(input == NULL) {

        printf("malloc error.\n");
        exit(1);
    }
    inpRef = input;

    // Set defaults
    for(i=0; i < KEY_BUFFER_SIZE; ++ i) {

        input->oldNormals[i] = 255;
        input->oldExt[i] = 255;
        input->normalRead[i] = false;
        input->extRead[i] = false;
    }

    // Hook handlers
    input->oldHandler = _dos_getvect(0x09);
    _dos_setvect(0x09, handler);

    return input;
}


// Destroy input manager
void destroyInputManager(InputManager* input) {

    if(input == NULL) return;

    // Unhook
    if (input->oldHandler != NULL) {

        _dos_setvect(0x09, input->oldHandler);
        input->oldHandler = NULL;
    }

    free(input);
}


// Get button
uint8 inputGetButton(InputManager* input, uint8 button) {

    uint8 ret = Up;

    if(input == NULL || button > MAX_BUTTON)
        return Up;

    // If arrow key
    if(button < 4) {

        // Get extended
        ret = getValueFromArr(
            input->extKeys, 
            input->extRead, button);
    }
    else {

        // Get normal
        ret = getValueFromArr(
            input->normalKeys, 
            input->normalRead, button);
    }
    return ret;
}
