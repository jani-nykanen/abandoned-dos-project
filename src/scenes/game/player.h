// Player object
// (c) 2019 Jani Nykänen

#ifndef __PLAYER__
#define __PLAYER__

#include "../../core/types.h"
#include "../../core/sprite.h"
#include "../../core/eventman.h"
#include "../../core/resources.h"

#include "gameobject.h"

// Constants
#define PL_GEM_MAX 5
#define PL_LIFE_MAX 6

// Initialize global player content
void plInit(ResourceList* res);

// Player type
typedef struct {

    IMPLEMENT_GAME_OBJECT

    // Starting position
    Vector2 startPos;

    // Direction
    int16 dir;
    // Sprite
    Sprite spr;
    // End frame
    int16 endFrame;
    // Old end frame
    int16 oldEndFrame;

    // Speed modificator
    int16 speedMod;

    // Lives
    int16 lives;
    // Gems
    int16 gems;
    
} Player;

// Create a player
Player plCreate(int16 x, int16 y);

// Update player
void plUpdate(Player* pl, EventManager* evMan, int16 steps);

// Draw player
void plDraw(Player* pl, Graphics* g);

// Stage collision
void plStageCollision(Player* pl, void* s, void* objm, int16 steps);

// Add a gem
void plAddGem(Player* pl);

#endif // __PLAYER__
