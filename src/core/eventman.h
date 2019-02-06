// Event manager
// (c) 2019 Jani Nykänen

#ifndef __EVMAN_H__
#define __EVMAN_H__

#include "input.h"

// Event manager
typedef struct {

    // Input manager
    InputManager* input;
    // Core
    void* core;

}EventManager;

// Create event manager
EventManager createEventManager(void* core,
    InputManager* input);

// Terminate
void terminate(EventManager* evMan);

#endif // __EVMAN_H__
