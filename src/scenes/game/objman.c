// (Game) object manager
// (c) 2019 Jani Nykänen

#include "objman.h"


// Initialize global content
void objmanInit(ResourceList* res) {

    // Initialize all the components
    plInit(res);
    initGem(res);
    initEnemy(res);
}


// Create an object manager
ObjectManager objmanCreate(Stage* s) {

    ObjectManager objm;

    // Set defaults
    objm.gemCount = 0;
    objm.enemyCount = 0;

    // Parse objects from the tilemap
    stageParseObjects(s, &objm);

    return objm;
}


// Update objects
void objmanUpdate(ObjectManager* objm, EventManager* evMan, Stage* s,
    int16 steps) {

    const int16 PL_W = 2;
    const int16 PL_H = 2;

    int16 i;

    // If stage transiting, update only
    // the player
    if(s->transiting) {

        plTransition(&objm->player, TRANS_SPEED, steps);
        return;
    }

    // Update gems
    for(i = 0; i < objm->gemCount; ++ i) {

        gemUpdate(&objm->gems[i], steps);
        gemPlayerCollision(&objm->gems[i], &objm->player);
    }

    // Update enemies
    for(i = 0; i < objm->enemyCount; ++ i) {

        enemyUpdate(&objm->enemies[i], &objm->player, steps);
    }

    // Update player
    plUpdate(&objm->player, evMan, steps);

    // Refresh player neighborhood
    stageRefreshNeighborhood(s, (GameObject*)&objm->player,
        PL_W, PL_H);

    plStageCollision(&objm->player, (void*)s, (void*)objm, steps);

}


// Draw objects
void objmanDraw(ObjectManager* objm, Stage* s, Graphics* g) {

    int16 i;

    // Translate, so we don't run out of pixels
    // (with precision 128 we cannot go too high)
    gMove(g, 304/2, 0);

    if(!s->transiting) {

        // Draw gems
        for(i = 0; i < objm->gemCount; ++ i) {

            gemDraw(&objm->gems[i], g);
        }

        // Draw enemies
        for(i = 0; i < objm->enemyCount; ++ i) {

            enemyDraw(&objm->enemies[i], g);
            enemyStageCollision(&objm->enemies[i], s);
        }
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


// Add an enemy
void objmanAddEnemy(ObjectManager* objm, int16 x, int16 y, uint8 id) {

    if(objm->enemyCount == ENEMY_MAX)
        return;

    objm->enemies[objm->enemyCount ++] = createEnemy(x, y, id);
}


// Clear objects (excluding player)
void objmanClearObjects(ObjectManager* objm) {

    objm->gemCount = 0;
    objm->enemyCount = 0;
}
