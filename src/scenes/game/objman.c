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
    objm.player = plCreate(0, 88);

    return objm;
}


// Update objects
void objmanUpdate(ObjectManager* objm, EventManager* evMan, 
    int16 steps) {

    // Update player
    plUpdate(&objm->player, evMan, steps);
}


// Update stage-related things
// (=set drawable tiles)
void objmanUpdateStage(ObjectManager* objm, Stage* s) {

    const int16 PL_RADIUS = 1;

    // Refresh player neighborhood
    stageRefreshNeighborhood(s, (GameObject*)&objm->player,
        PL_RADIUS, PL_RADIUS);
}


// Draw objects
void objmanDraw(ObjectManager* objm, Graphics* g) {


    // Translate, so we don't run out of pixels
    // (with precision 128 we cannot go too high)
    gMove(g, 304/2, 0);

    // Draw player
    plDraw(&objm->player, g);
}
