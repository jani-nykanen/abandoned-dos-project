// Event manager
// (c) 2019 Jani Nykänen

#include "eventman.h"


// Create event manager
EventManager createEventManager(InputManager* input) {

    EventManager evMan;

    evMan.input = input;

    return evMan;
}
