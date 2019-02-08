// Event manager
// (c) 2019 Jani Nykänen

#ifndef __EVMAN_H__
#define __EVMAN_H__

#include "input.h"
#include "vpad.h"

// Event manager
typedef struct {

    // Input manager
    InputManager* input;
    // Virtual gamepad
    Vpad* vpad;
    // Core
    void* core;

}EventManager;

// Create event manager
EventManager createEventManager(void* core,
    InputManager* input, Vpad* vpad);

// Terminate
void eventTerminate(EventManager* evMan);

#endif // __EVMAN_H__
