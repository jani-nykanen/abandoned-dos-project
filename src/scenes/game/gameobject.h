// A game object base class
// (c) 2019 Jani Nykänen

#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include "../../core/types.h"

#define IMPLEMENT_GAME_OBJECT \
    Vector2 pos; \
    Vector2 speed; \
    Vector2 target;

// Base game object type
typedef struct {

    IMPLEMENT_GAME_OBJECT

} GameObject;


#endif // __GAMEOBJECT__
