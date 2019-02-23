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
    objm.player = plCreate(-152+16, 88);

    return objm;
}


// Update objects
void objmanUpdate(ObjectManager* objm, EventManager* evMan, Stage* s,
    int16 steps) {

    const int16 PL_W = 1;
    const int16 PL_H = 2;

    // Update player
    plUpdate(&objm->player, evMan, steps);

    // Update collisions
    stageCollision(s, (GameObject*)&objm->player, steps);

    // Refresh player neighborhood
    stageRefreshNeighborhood(s, (GameObject*)&objm->player,
        PL_W, PL_H);

}


// Draw objects
void objmanDraw(ObjectManager* objm, Graphics* g) {


    // Translate, so we don't run out of pixels
    // (with precision 128 we cannot go too high)
    gMove(g, 304/2, 0);

    // Draw player
    plDraw(&objm->player, g);
}
