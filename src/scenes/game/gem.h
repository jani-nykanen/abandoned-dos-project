// Gem
// (c) 2019 Jani Nykänen

#ifndef __GEM__
#define __GEM__

#include "player.h"

// Initialize global content
void initGem(ResourceList* res);

// Gem type
typedef struct {

    // Position
    Vector2 pos;
    // Sprite
    Sprite spr;
    // Does exist
    bool exist;

} Gem;

// Create a gem
Gem createGem(int16 x, int16 y);

// Update gem
void gemUpdate(Gem* gem, int16 steps);

// Player collision
void gemPlayerCollision(Gem* gem, Player* pl);

// Draw gem
void gemDraw(Gem* gem, Graphics* g);


#endif // __GEM__
