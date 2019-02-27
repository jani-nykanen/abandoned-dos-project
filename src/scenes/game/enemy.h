// Enemy base type
// (c) 2019 Jani Nykänen

#ifndef __ENEMY__
#define __ENEMY__

#include "player.h"

// Enemy type
typedef struct {

    // Position & speed
    Vector2 pos;
    Vector2 speed;
    // Sprite
    Sprite spr;

    // ID
    uint8 id;

    // Does exist
    bool exist;

} Enemy;


#endif // __ENEMY__
