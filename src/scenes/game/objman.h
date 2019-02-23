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
ObjectManager objmanCreate(Stage* s);

// Update objects
void objmanUpdate(ObjectManager* objm, 
    EventManager* evMan, Stage* stage,
    int16 steps);

// Draw objects
void objmanDraw(ObjectManager* objm, Graphics* g);

#endif // __OBJMAN__
