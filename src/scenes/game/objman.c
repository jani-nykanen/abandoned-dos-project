// (Game) object manager
// (c) 2019 Jani Nykänen

#include "objman.h"


// Initialize global content
void objmanInit(ResourceList* res) {

    // Initialize all the components
    plInit(res);
}


// Create an object manager
ObjectManager objmanCreate() {

    ObjectManager objm;

    // Create components
    objm.player = plCreate(104, 88);

    return objm;
}


// Update objects
void objmanUpdate(ObjectManager* objm, EventManager* evMan, 
    int16 steps) {

    // Update player
    plUpdate(&objm->player, evMan, steps);
}


// Draw objects
void objmanDraw(ObjectManager* objm, Graphics* g) {

    // Draw player
    plDraw(&objm->player, g);
}
