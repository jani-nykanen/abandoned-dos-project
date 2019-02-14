// Player object
// (c) 2019 Jani Nykänen

#include "player.h"

#include <stdio.h>

#include "../../util/mathext.h"

// Bitmaps
static Bitmap* bmpPlayer;


// Initialize global player content
void plInit(ResourceList* res) {

    // Get bitmaps
    bmpPlayer = (Bitmap*)rsGetResource(res, "player");
}


// Control
static void plControl(Player* pl, EventManager* evMan, int16 steps) {

    Vpad* vpad = evMan->vpad;

    pl->target.x = vpad->stick.x;
    pl->target.y = vpad->stick.y;
}


// Move
static void plMove(Player* pl, int16 steps) {

    pl->speed.x = pl->target.x;
    pl->speed.y = pl->target.y;

    pl->pos.x += pl->speed.x * steps;
    pl->pos.y += pl->speed.y * steps;

    // Determine direction
    if(pl->speed.x != 0 || pl->speed.y != 0) {

        if( extAbs(pl->speed.x) > extAbs(pl->speed.y) ) {

            if(pl->speed.x > 0)
                pl->dir = 2;

            else
                pl->dir = 3;
        }
        else {

            if(pl->speed.y > 0)
                pl->dir = 0;

            else
                pl->dir = 1;
        }
    }
}


// Animate
static void plAnimate(Player* pl, int16 steps) {

    int16 dir = pl->dir < 3 ? pl->dir : 2;

    // Moving
    if(pl->speed.x != 0 || pl->speed.y != 0) {

        sprAnimate(&pl->spr, dir, 0, 3, 8, steps);
    }
    // Standing
    else {

        sprAnimate(&pl->spr, dir, 0, 0, 0, steps);
    }
}


// Create a player
Player plCreate(int16 x, int16 y) {

    Player pl;

    pl.pos.x = x * FIXED_PREC;
    pl.pos.y = y * FIXED_PREC;

    // Set defaults
    pl.speed.x = 0;
    pl.speed.y = 0;
    pl.target = pl.speed;
    pl.dir = 0;

    // Create sprite
    pl.spr = sprCreate(16, 16);

    return pl;
}


// Update player
void plUpdate(Player* pl, EventManager* evMan, int16 steps) {

    // Control
    plControl(pl, evMan, steps);
    // Move
    plMove(pl, steps);
    // Animate
    plAnimate(pl, steps);
}


// Draw player
void plDraw(Player* pl, Graphics* g) {

    sprDraw(&pl->spr, g, bmpPlayer, 
        (pl->pos.x/FIXED_PREC)-8,
        (pl->pos.y/FIXED_PREC)-16,
        pl->dir == 3
        );
}
