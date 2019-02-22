// Player object
// (c) 2019 Jani Nykänen

#ifndef __PLAYER__
#define __PLAYER__

#include "../../core/types.h"
#include "../../core/sprite.h"
#include "../../core/eventman.h"
#include "../../core/resources.h"

#include "gameobject.h"

// Initialize global player content
void plInit(ResourceList* res);

// Player type
typedef struct {

    IMPLEMENT_GAME_OBJECT

    // Direction
    int16 dir;
    // Sprite
    Sprite spr;

} Player;

// Create a player
Player plCreate(int16 x, int16 y);

// Update player
void plUpdate(Player* pl, EventManager* evMan, int16 steps);

// Draw player
void plDraw(Player* pl, Graphics* g);

#endif // __PLAYER__
