// Gem
// (c) 2019 Jani Nykänen

#include "gem.h"

// Bitmaps
static Bitmap* bmpGem;


// Initialize global content
void initGem(ResourceList* res) {

    bmpGem = (Bitmap*)rsGetResource(res, "gem");
}


// Create a gem
Gem createGem(int16 x, int16 y) {

    Gem gem;

    gem.exist = true;
    gem.pos = vec2(x, y);
    gem.spr = createSprite(16, 16);

    return gem;
}


// Update gem
void gemUpdate(Gem* gem, int16 steps) {

    const int16 ANIM_SPEED = 8;

    if(!gem->exist) return;

    // Animate
    sprAnimate(&gem->spr, 0, 0, 4, ANIM_SPEED, steps);
}


// Player collision
void gemPlayerCollision(Gem* gem, Player* pl) {

    int16 px, py;
    int16 w = pl->width;
    int16 h = pl->height;

    if(!gem->exist) return;

    // Check collision
    px = pl->pos.x / FIXED_PREC;
    py = pl->pos.y / FIXED_PREC;

    if(px+w/2 >= gem->pos.x && px-w/2 <= gem->pos.x+16
       && py >= gem->pos.y && py-h <= gem->pos.y+16 ) {

        gem->exist = false;
        plAddGem(pl);
    }
}


// Draw gem
void gemDraw(Gem* gem, Graphics* g) {

    if(!gem->exist) return;

    // Draw sprite
    sprDrawFast(&gem->spr, g, bmpGem, gem->pos.x, gem->pos.y);
}
