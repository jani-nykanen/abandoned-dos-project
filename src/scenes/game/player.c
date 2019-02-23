// Player object
// (c) 2019 Jani Nykänen

#include "player.h"

#include <stdio.h>

#include "../../util/mathext.h"

// Bitmaps
static Bitmap* bmpRat;


// Initialize global player content
void plInit(ResourceList* res) {

    // Get bitmaps
    bmpRat = (Bitmap*)rsGetResource(res, "rat");
}


// Control
static void plControl(Player* pl, EventManager* evMan, int16 steps) {

    const int16 GRAVITY_TARGET = 2 * FIXED_PREC;
    const int16 DELTA = FIXED_PREC / 4;

    Vpad* vpad = evMan->vpad;
    uint16 targetFrame;

    // Set horizontal target
    if(pl->canJump) {

        pl->speed.x = 0;
        pl->target.x = 0;
    }
    else {
        
        pl->target.x = vpad->stick.x;
    }

    // Determine the end frame
    if(extAbs(vpad->stick.y) > DELTA) {

        targetFrame = vpad->stick.y > 0 ? 2 : 7;
    }
    else {
        
        targetFrame = 4;
    }
    if(!pl->canJump || targetFrame > pl->spr.frame) {

        pl->endFrame = targetFrame;
    }

    // Set gravity
    pl->target.y = GRAVITY_TARGET;
}


// Move
static void plMove(Player* pl, int16 steps) {

    const int16 ACC_X = FIXED_PREC / 16;
    const int16 GRAVITY = FIXED_PREC / 16;

    // Update axes
    gobjUpdateAxis(&pl->pos.x, &pl->speed.x,
        pl->target.x, ACC_X, steps);
    gobjUpdateAxis(&pl->pos.y, &pl->speed.y,
        pl->target.y, GRAVITY, steps);   

    // Determine direction
    if(pl->target.x != 0)
        pl->dir = pl->target.x < 0 ? -1 : 1;
}


// Animate
static void plAnimate(Player* pl, int16 steps) {

    const int16 JUMP_HEIGHT[] = {
        0, 0, -192, 0,
        -256, 0, 0, -320
    };
    const int16 ANIM_SPEED = 4;
    const int16 RESTORE_SPEED = 1;

    if(pl->canJump) {

        // Bounce
        sprAnimate(&pl->spr, 0, 0, pl->endFrame+1, ANIM_SPEED, steps);
        if(pl->spr.frame == pl->endFrame+1) {

            pl->spr.frame = pl->endFrame;
            pl->spr.count = 0;
            pl->speed.y = JUMP_HEIGHT[pl->endFrame];
            pl->canJump = false;

            pl->oldEndFrame = pl->endFrame;
            pl->endFrame = 4;
        }
    }
    else if(pl->spr.frame > 0) {

        // Animate back
        sprAnimate(&pl->spr, 0, pl->oldEndFrame, 
            0, RESTORE_SPEED, steps);
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
    pl.canJump = false;
    pl.endFrame = 4;
    
    // Collision box
    pl.width = 4;
    pl.height = 16;

    // Create sprite
    pl.spr = sprCreate(16, 24);

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

    pl->canJump = false;
}


// Draw player
void plDraw(Player* pl, Graphics* g) {

    sprDraw(&pl->spr, g, bmpRat, 
        (pl->pos.x/FIXED_PREC)-8,
        (pl->pos.y/FIXED_PREC)-24,
        pl->dir == -1
        );
}

