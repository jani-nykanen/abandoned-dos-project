// Enemy base type
// (c) 2019 Jani Nykänen

#ifndef __ENEMY__
#define __ENEMY__

#include "player.h"
#include "stage.h"

// Initialize global content
void initEnemy(ResourceList* res);

// Enemy type
typedef struct {

    // Basic stuff
    Vector2 pos;
    Vector2 startPos;
    Vector2 speed; 
    Vector2 target; 

    // Dimensions
    int16 width; 
    int16 height; 

    // Death variables
    bool dying;
    int16 deathTimer;

    // Sprite
    Sprite spr;
    // Flip
    bool flip;

    // ID
    uint8 id;
    // Special timer
    int16 spcTimer;

    // Does exist
    bool exist;

} Enemy;

// Create an enemy
Enemy createEnemy(int16 x, int16 y, uint8 id);

// Update an enemy
void enemyUpdate(Enemy* e, Player* pl, int16 steps);

// Draw an enemy
void enemyDraw(Enemy* e, Graphics* g);

// Stage collision
void enemyStageCollision(Enemy* e, Stage* s);

// "Collider block" collision
void enemyBlockCollision(Enemy* e, int16 x, int16 y, int16 w, int16 h);

#endif // __ENEMY__
