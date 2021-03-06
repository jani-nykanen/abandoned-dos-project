// (Game) object manager
// (c) 2019 Jani Nykänen

#ifndef __OBJMAN__
#define __OBJMAN__

#include "player.h"
#include "stage.h"
#include "gem.h"
#include "enemy.h"

#define GEM_MAX 4
#define ENEMY_MAX 6

// Initialize global content
void objmanInit(ResourceList* res);

// Object manager type
typedef struct {

    // Components
    Player player;
    Gem gems [GEM_MAX];
    Enemy enemies [ENEMY_MAX];

    // Objects counts
    uint8 gemCount;
    uint8 enemyCount;


} ObjectManager;

// Create an object manager
ObjectManager objmanCreate(Stage* s);

// Update objects
void objmanUpdate(ObjectManager* objm, 
    EventManager* evMan, Stage* stage,
    int16 steps);

// Draw objects
void objmanDraw(ObjectManager* objm, Stage* s, Graphics* g);

// Add a gem
void objmanAddGem(ObjectManager* objm, int16 x, int16 y);

// Add an enemy
void objmanAddEnemy(ObjectManager* objm, int16 x, int16 y, uint8 id);

// Clear objects (excluding player)
void objmanClearObjects(ObjectManager* objm);

#endif // __OBJMAN__
