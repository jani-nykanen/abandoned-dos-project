// Event manager
// (c) 2019 Jani Nykänen

#include "eventman.h"

#include "core.h"


// Create event manager
EventManager createEventManager(void* core, 
    InputManager* input, Vpad* vpad) {

    EventManager evMan;

    evMan.input = input;
    evMan.core = core;
    evMan.vpad = vpad;

    return evMan;
}


// Terminate
void eventTerminate(EventManager* evMan) {

    coreTerminate((Core*)evMan->core);
}
