// Enemy base type
// (c) 2019 Jani Nykänen

#include "enemy.h"

#include "game.h"

#include "../../util/mathext.h"


// Constants
static const int16 DEATH_INTERVAL = 2;
static const int16 DEATH_MAX = 8;

// Bitmap
static Bitmap* bmpEnemy;


// Initialize global content
void initEnemy(ResourceList* res) {

    // Get bitmaps
    bmpEnemy = (Bitmap*)rsGetResource(res, "enemy");
}


// Initialize enemy by id
static void enemyInitSpecific(Enemy* e, int16 x, int16 y) {

    const int16 WALKER_SPEED = FIXED_PREC / 3;
    const int16 GHOST_SPEED = FIXED_PREC / 4;

    // Default speed to zero
    e->speed.x = 0;
    e->speed.y = 0;

    switch (e->id) {

        // Walker & bird
        case 0:
        case 2:
            e->speed.x = WALKER_SPEED * (x % 2 == 0 ? 1 : -1);
            e->pos.y += FIXED_PREC;
            break;
        
        // Ghost
        case 1:
            e->speed.y = GHOST_SPEED * (y % 2 == 0 ? 1 : -1);
            break;
    
        default:
            break;
    }
}



// Die
static void enemyDie(Enemy* e, int16 steps) {

    e->deathTimer += steps;
    if(e->deathTimer >= DEATH_INTERVAL * DEATH_MAX) {

        e->exist = false;
    }
}


// Player collision
static void enemyPlayerCollision(Enemy* e, Player* pl, int16 steps) {

    const int16 HURT_AREA_TOP = 1;
    const int16 HURT_AREA_BOTTOM = 4;
    const int16 PL_JUMP_HEIGHT_BASE = -192;
    const int16 PL_HEIGHT_MUL = -48;
    const int16 HURT_AREA_OFFSET = 2;

    int16 ex = e->pos.x/FIXED_PREC - e->width/2;
    int16 ey = e->pos.y/FIXED_PREC - e->height;

    int16 px = pl->pos.x/FIXED_PREC - pl->width/2;
    int16 py = pl->pos.y/FIXED_PREC;

    // No collision with a death playre
    if(pl->dying) return;

    if(pl->speed.y >= 0 && 
        px+pl->width >= ex && px <= ex+e->width && 
        py >= ey-HURT_AREA_TOP*steps &&
        py <= ey + (HURT_AREA_BOTTOM + pl->speed.y/FIXED_PREC)*steps) {

        e->dying = true;
        e->deathTimer = 0;
        
        pl->speed.y = PL_JUMP_HEIGHT_BASE + PL_HEIGHT_MUL * pl->heightMod;

        return;
    }

    // Hurt collision
    gobjHurtCollision((GameObject*)pl, 
        ex+HURT_AREA_OFFSET, 
        ey+HURT_AREA_BOTTOM, 
        e->width-HURT_AREA_OFFSET*2, 
        e->height-HURT_AREA_BOTTOM);
}


// Move
static void enemyMove(Enemy* e, int16 steps) {

    const int16 PROP_TIMER_SPEED = 2;
    const int16 PROP_AMPLITUDE = 8;

    switch (e->id) {

        // "Uniformly" moving objects
        case 0:
        case 1:
        case 2:
            e->pos.x += e->speed.x * steps;
            e->pos.y += e->speed.y * steps;
            break;

        // Propeller-thing
        case 3:

            e->spcTimer += PROP_TIMER_SPEED * steps;
            e->spcTimer %= 360;
            e->pos.y = e->startPos.y + fixedSin(e->spcTimer) * PROP_AMPLITUDE;
            break;

        default:
            break;
    }
}


// Animate
static void enemyAnimate(Enemy* e, int16 steps) {

    const int WALK_SPEED = 8;
    const int GHOST_SPEED = 10;
    const int PROPELLER_SPEED = 6;

    int16 animSpeed =0;
    int16 endFrame = 3;

    switch (e->id) {

        // Walker & bird
        case 0:
        case 2:
            animSpeed = WALK_SPEED;
            e->flip = e->speed.x > 0;
            break;
        
        // Ghost
        case 1:
            animSpeed = GHOST_SPEED;
            endFrame = 2;
            break;

        // "Propeller"
        case 3:
            animSpeed = PROPELLER_SPEED;
            break;
    
        default:
            break;
    }

    // Animate
    sprAnimate(&e->spr, e->id, 0, endFrame, animSpeed, steps);
}


// Create an enemy
Enemy createEnemy(int16 x, int16 y, uint8 id) {

    Enemy e;
    e.pos = vec2((x*16+8 - VIEW_WIDTH/2) * FIXED_PREC,
        (y+1)*16 * FIXED_PREC);
    e.startPos = e.pos;

    e.id = id;

    // Set defaults
    e.width = 16;
    e.height = 16;
    e.exist = true;
    e.dying = false;
    e.deathTimer = 0;
    e.flip = false;
    e.spcTimer = 0;

    // Create sprite
    e.spr = createSprite(16, 16);
    e.spr.frame = 0;
    e.spr.row = (int16)id;

    // Initialize specific behavior
    enemyInitSpecific(&e, x, y);

    return e;
}


// Update an enemy
void enemyUpdate(Enemy* e, Player* pl, int16 steps) {

    if(!e->exist) return;

    // If dying, die
    if(e->dying) {

        enemyDie(e, steps);
        return;
    }

    // Move
    enemyMove(e, steps);
    // Animate
    enemyAnimate(e, steps);

    // Player collision
    enemyPlayerCollision(e, pl, steps);
}


// Draw an enemy
void enemyDraw(Enemy* e, Graphics* g) {

    int16 skip;

    if(!e->exist) return;

    // Make appear or disappear
    if(e->dying) {

        skip = DEATH_MAX - e->deathTimer / DEATH_INTERVAL;
        if(skip == 0) return;

        sprDrawSkip(&e->spr, g, bmpEnemy, 
            (e->pos.x/FIXED_PREC)-8,
            (e->pos.y/FIXED_PREC)-16,
            skip,
            e->flip
            );
    }
    // Draw basic sprite
    else {

        sprDraw(&e->spr, g, bmpEnemy, 
            (e->pos.x/FIXED_PREC)-8,
            (e->pos.y/FIXED_PREC)-16,
            e->flip
            );
    }
}


// Stage collision
void enemyStageCollision(Enemy* e, Stage* s) {

    if(!e->exist) return;

    stageRefreshNeighborhood(s, (GameObject*)e, 2, 1);

    // Base collisions
    stageEnemyCollision(s, (void*)e);

    // Upper screen collision
    if(e->speed.y < 0) {

        enemyBlockCollision(e, -VIEW_WIDTH/2, -16, VIEW_WIDTH, 16);
    }
}


// "Collider block" collision
void enemyBlockCollision(Enemy* e, 
    int16 x, int16 y, int16 w, int16 h) {

    const int16 OFFSET = 2;

    int16 px = e->pos.x / FIXED_PREC;
    int16 py = e->pos.y / FIXED_PREC;

    int16 mx = x + w/2;
    int16 my = y + h/2;

    // If not inside the block
    if(px-e->width/2 >= x+w-OFFSET || px+e->width/2 <= x+OFFSET || 
       py <= y+OFFSET || py-e->height >= y+h-OFFSET)
        return;

    // Left & right
    if( (e->speed.x < 0 && px > mx) ||
        (e->speed.x > 0 && px < mx) ) {
        
        e->speed.x *= -1;
    }
    // Up & down
    if( (e->speed.y < 0 && py > my) ||
        (e->speed.y > 0 && py < my) ) {
        
        e->speed.y *= -1;
    }

}
