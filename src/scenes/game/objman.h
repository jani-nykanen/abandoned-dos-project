// (Game) object manager
// (c) 2019 Jani Nykänen

#ifndef __OBJMAN__
#define __OBJMAN__

#include "player.h"
#include "stage.h"
#include "gem.h"

#define GEM_MAX 16

// Initialize global content
void objmanInit(ResourceList* res);

// Object manager type
typedef struct {

    // Components
    Player player;
    Gem gems [GEM_MAX];

    // Objects counts
    int16 gemCount;


} ObjectManager;

// Create an object manager
ObjectManager objmanCreate(Stage* s);

// Update objects
void objmanUpdate(ObjectManager* objm, 
    EventManager* evMan, Stage* stage,
    int16 steps);

// Draw objects
void objmanDraw(ObjectManager* objm, Graphics* g);

// Add a gem
void objmanAddGem(ObjectManager* objm, int16 x, int16 y);

#endif // __OBJMAN__
