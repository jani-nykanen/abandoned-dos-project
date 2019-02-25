// Player object
// (c) 2019 Jani Nykänen

#include "player.h"

#include <stdio.h>

#include "../../util/mathext.h"

#include "game.h"
#include "stage.h"
#include "objman.h"

// Bitmaps
static Bitmap* bmpRat;


// Initialize global player content
void plInit(ResourceList* res) {

    // Get bitmaps
    bmpRat = (Bitmap*)rsGetResource(res, "rat");
}


// Compute target x
static void plComputeHorizontalTarget(Player* pl, EventManager* evMan) {

    const int16 TARGET_DIV = 8;

    Vpad* vpad = evMan->vpad;
    pl->target.x = vpad->stick.x / TARGET_DIV * pl->speedMod;
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
        
        plComputeHorizontalTarget(pl, evMan);
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


// Die
static void plDie(Player* pl) {

    // Reset
    pl->pos = pl->startPos;
    pl->speed = vec2(0, 0);
    pl->target = pl->speed;
    pl->spr.frame = 0;
    pl->spr.row = 0;
    pl->dir = 1;

    // Reduce life, if any
    if(pl->lives > 0) {

        -- pl->lives;
        gameRefreshInfo();
    }
    else {

        // ...
    }
}
// Die callback
static void _dieCB(void* gobj) {
    plDie((Player*)gobj);
}


// Move
static void plMove(Player* pl, int16 steps) {

    const int16 ACC = FIXED_PREC / 32;
    const int16 GRAVITY = FIXED_PREC / 16;
    const int16 HEIGHT = 24;

    // Update axes
    gobjUpdateAxis(&pl->pos.x, &pl->speed.x,
        pl->target.x, ACC, steps);
    gobjUpdateAxis(&pl->pos.y, &pl->speed.y,
        pl->target.y, GRAVITY, steps);   

    // Determine direction
    if(pl->target.x != 0)
        pl->dir = pl->target.x < 0 ? -1 : 1;

    // Outside the room boundary, die
    if(pl->pos.y/FIXED_PREC - HEIGHT > VIEW_HEIGHT) {

        plDie(pl);
    }
}


// Animate
static void plAnimate(Player* pl, EventManager* evMan, int16 steps) {

    const int16 JUMP_HEIGHT[] = {
        0, 0, -192, 0,
        -256, 0, 0, -336
    };
    const int16 SPEED_MOD[] = {
        0, 0, 12, 0,
        6, 0, 0, 5
    };
    const int16 ANIM_SPEED = 4;
    const int16 RESTORE_SPEED = 2;

    if(pl->canJump) {

        // Bounce
        sprAnimate(&pl->spr, 0, 0, pl->endFrame+1, ANIM_SPEED, steps);
        if(pl->spr.frame == pl->endFrame+1) {

            // Reset animation
            pl->spr.frame = pl->endFrame;
            pl->spr.count = 0;

            // Set vertical speed
            pl->speed.y = JUMP_HEIGHT[pl->endFrame];
            pl->canJump = false;

            // Set horizontal speed
            pl->speedMod = SPEED_MOD[pl->endFrame];
            plComputeHorizontalTarget(pl, evMan);
            pl->speed.x = pl->target.x;

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

    // Set initial position & store it
    pl.pos.x = x * FIXED_PREC;
    pl.pos.y = y * FIXED_PREC;
    pl.startPos = pl.pos;

    // Set defaults
    pl.speed.x = 0;
    pl.speed.y = 0;
    pl.target = pl.speed;
    pl.dir = 0;
    pl.canJump = false;
    pl.endFrame = 4;
    pl.speedMod = 4;

    // Default lives & gems
    pl.lives = PL_LIFE_MAX -2;
    pl.gems = 0;
    
    // Collision box
    pl.width = 4;
    pl.height = 16;

    // Set callbacks
    pl.hurtCB = _dieCB;

    // Create sprite
    pl.spr = createSprite(16, 24);

    return pl;
}


// Update player
void plUpdate(Player* pl, EventManager* evMan, int16 steps) {

    // Control
    plControl(pl, evMan, steps);
    // Move
    plMove(pl, steps);
    // Animate
    plAnimate(pl, evMan, steps);

    pl->canJump = false;
}


// Update player transition when the stage
// is transiting
void plTransition(Player* pl, int16 speed, int16 steps) {

    pl->pos.x -= speed * steps * FIXED_PREC;
    if(pl->pos.x/FIXED_PREC < -VIEW_WIDTH/2 + pl->spr.width/2) {

        pl->pos.x = (-VIEW_WIDTH/2 + pl->spr.width/2) * FIXED_PREC;     
    }
}


// Draw player
void plDraw(Player* pl, Graphics* g) {

    sprDraw(&pl->spr, g, bmpRat, 
        (pl->pos.x/FIXED_PREC)-8,
        (pl->pos.y/FIXED_PREC)-24,
        pl->dir == -1
        );
}


// Stage collision
void plStageCollision(Player* pl, void* _s, void* _objm, int16 steps) {

    Stage* s = _s;
    ObjectManager* objm = (ObjectManager*)_objm;

    // Left-side collision
    gobjWallCollision((GameObject*)pl, 
        -VIEW_WIDTH/2, -16, VIEW_HEIGHT+32, -1, steps);

    // Tile collisions
    stageCollision(s, (GameObject*) pl, steps);

    // Right-side collision
    if(pl->pos.x/FIXED_PREC+pl->width/2 > VIEW_WIDTH/2) {

        ++ s->roomIndex;

        // Clear objects
        objmanClearObjects(objm);
        // Parse new objects
        stageParseObjects(s, objm);

        // Move player to the left
        // pl->pos.x -= (VIEW_WIDTH-pl->width/2) * FIXED_PREC;
        // Set speed to zero
        pl->speed.x = 0;
        pl->dir = 1;

        -- s->roomIndex;

        // Start transition to the next stage
        stageSetTransition(s);
    }
    
}


// Add a gem
void plAddGem(Player* pl) {

    if(++ pl->gems >= PL_GEM_MAX) {

        pl->gems = 0;
        if(pl->lives < PL_LIFE_MAX)
            ++ pl->lives; 
    }

    // Refresh info
    gameRefreshInfo();
}


// Set a new starting position
void plSetStartPos(Player* pl, int16 x, int16 y) {

    pl->startPos.x = x * FIXED_PREC;
    pl->startPos.y = y * FIXED_PREC;
}
