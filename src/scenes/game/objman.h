// (Game) object manager
// (c) 2019 Jani Nykänen

#ifndef __OBJMAN__
#define __OBJMAN__

#include "player.h"
#include "stage.h"

// Initialize global content
void objmanInit(ResourceList* res);

// Object manager type
typedef struct {

    // Components
    Player player;

} ObjectManager;

// Create an object manager
ObjectManager objmanCreate();

// Update objects
void objmanUpdate(ObjectManager* objm, EventManager* evMan, int16 steps);

// Update stage-related things
// (=set drawable tiles)
void objmanUpdateStage(ObjectManager* objm, Stage* s);

// Draw objects
void objmanDraw(ObjectManager* objm, Graphics* g);

#endif // __OBJMAN__
