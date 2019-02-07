// Event manager
// (c) 2019 Jani Nykänen

#include "eventman.h"

#include "core.h"


// Create event manager
EventManager createEventManager(void* core, 
    InputManager* input) {

    EventManager evMan;

    evMan.input = input;
    evMan.core = core;

    return evMan;
}


// Terminate
void eventTerminate(EventManager* evMan) {

    coreTerminate((Core*)evMan->core);
}
