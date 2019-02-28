// Enemy base type
// (c) 2019 Jani Nykänen

#include "enemy.h"

#include "game.h"

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
    const int16 FLY_SPEED = FIXED_PREC / 3;

    // Default speed to zero
    e->speed.x = 0;
    e->speed.y = 0;

    switch (e->id) {

        // Walker
        case 0:
            e->speed.x = WALKER_SPEED * (x % 2 == 0 ? 1 : -1);
            e->pos.y += FIXED_PREC;
            break;
        
        // Fly
        case 1:
            e->speed.y = FLY_SPEED * (y % 2 == 0 ? 1 : -1);
            break;
    
        default:
            break;
    }
}



// Die
static void enemyDie(Enemy* e, int16 steps) {

    // ...
}


// Player collision
static void enemyPlayerCollision(Enemy* e, Player* pl, int16 steps) {

    // ...
}


// Move
static void enemyMove(Enemy* e, int16 steps) {

    switch (e->id) {

        // "Uniformly" moving objects
        case 0:
        case 1:
            e->pos.x += e->speed.x * steps;
            e->pos.y += e->speed.y * steps;
            break;

        default:
            break;
    }
}


// Animate
static void enemyAnimate(Enemy* e, int16 steps) {

    const int WALK_SPEED = 8;
    const int FLY_SPEED = 8;

    int16 animSpeed =0;

    switch (e->id) {

        // Walker
        case 0:
            animSpeed = WALK_SPEED;
            e->flip = e->speed.x > 0;
            break;
        
        // Fly
        case 1:
            animSpeed = FLY_SPEED;
            break;
    
        default:
            break;
    }

    // Animate
    sprAnimate(&e->spr, e->id, 0, 3, animSpeed, steps);
}


// Create an enemy
Enemy createEnemy(int16 x, int16 y, uint8 id) {

    Enemy e;
    e.pos = vec2((x*16+8 - VIEW_WIDTH/2) * FIXED_PREC,
        (y+1)*16 * FIXED_PREC);

    e.id = id;

    // Set defaults
    e.width = 16;
    e.height = 16;
    e.exist = true;
    e.dying = false;
    e.deathTimer = 0;
    e.flip = false;

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

    int16 px = e->pos.x / FIXED_PREC;
    int16 py = e->pos.y / FIXED_PREC;

    int16 mx = x + w/2;
    int16 my = y + h/2;

    // If not inside the block
    if(px-e->width/2 >= x+w || px+e->width/2 <= x || py <= y || py-e->height >= y+h)
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
