// Event manager
// (c) 2019 Jani Nykänen

#ifndef __EVMAN_H__
#define __EVMAN_H__

#include "input.h"

// Event manager
typedef struct {

    // Input manager
    InputManager* input;

}EventManager;

// Create event manager
EventManager createEventManager(InputManager* input);

#endif // __EVMAN_H__
