// A game object base class
// (c) 2019 Jani Nykänen

#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include "../../core/types.h"

#include <stdbool.h>

#define IMPLEMENT_GAME_OBJECT \
    Vector2 pos; \
    Vector2 speed; \
    Vector2 target; \
    int16 width; \
    int16 height; \
    bool canJump; \
    void (*hurtCB) (void*); \
    bool dying; \
    int16 deathTimer;

// Base game object type
typedef struct {

    IMPLEMENT_GAME_OBJECT

} GameObject;


// Floor collision
bool gobjFloorCollision(GameObject* gobj, 
    int16 x, int16 y, int16 w, int16 steps);

// Wall collision
bool gobjWallCollision(GameObject* gobj,
    int16 x, int16 y, int16 h, int16 dir, 
    int16 steps);

// Ceiling collision
bool gobjCeilingCollision(GameObject* gobj, 
    int16 x, int16 y, int16 w, int16 steps);

// Hurt collision
bool gobjHurtCollision(GameObject* gobj, 
    int16 x, int16 y, int16 w, int16 h);

// Update axis
void gobjUpdateAxis(int16* axis, int16* speed, int16 target, 
    int16 acc, int16 steps);

#endif // __GAMEOBJECT__
