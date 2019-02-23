// (Game) object manager
// (c) 2019 Jani Nykänen

#include "objman.h"


// Initialize global content
void objmanInit(ResourceList* res) {

    // Initialize all the components
    plInit(res);
    initGem(res);
}


// Create an object manager
ObjectManager objmanCreate(Stage* s) {

    ObjectManager objm;

    // Create components
    objm.player = plCreate(-152+16, 88);

    // Set defaults
    objm.gemCount = 0;

    // Parse objects from the tilemap
    stageParseObjects(s, &objm);

    return objm;
}


// Update objects
void objmanUpdate(ObjectManager* objm, EventManager* evMan, Stage* s,
    int16 steps) {

    const int16 PL_W = 1;
    const int16 PL_H = 2;

    int16 i;

    // Update gems
    for(i = 0; i < objm->gemCount; ++ i) {

        gemUpdate(&objm->gems[i], steps);
        gemPlayerCollision(&objm->gems[i], &objm->player);
    }

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

    int16 i;

    // Translate, so we don't run out of pixels
    // (with precision 128 we cannot go too high)
    gMove(g, 304/2, 0);

    // Draw gems
    for(i = 0; i < objm->gemCount; ++ i) {

        gemDraw(&objm->gems[i], g);
    }

    // Draw player
    plDraw(&objm->player, g);
}


// Add a gem
void objmanAddGem(ObjectManager* objm, int16 x, int16 y) {

    if(objm->gemCount == GEM_MAX)
        return;

    objm->gems[objm->gemCount ++] = createGem(x, y);
}
