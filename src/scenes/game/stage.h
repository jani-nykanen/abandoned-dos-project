// Stage
// (c) 2019 Jani Nykänen

#ifndef __STAGE__
#define __STAGE__

#include "../../core/tilemap.h"
#include "../../core/graphics.h"

#include "gameobject.h"

#include <stdbool.h>

// Stage type
typedef struct {

    // Tilemap
    Tilemap* tmap;
    // Tileset
    Bitmap* bmpTileset;
    // Update buffer
    bool* updateBuffer;

    // Stage index
    uint8 index;

    // Water position
    int16 waterPos;
    // Wave "timer"
    int16 waveTimer;

} Stage;

// Create stage. Returns a pointer
// so that tilemap does not get
// lost
Stage* createStage(uint8 index, Bitmap* bmpTileset);

// Update stage
void stageUpdate(Stage* s, int16 steps);

// Draw stage
void stageDraw(Stage* s, Graphics* g);

// Refresh neighborhood to determine redrawable
// tiles
void stageRefreshNeighborhood(Stage* s, 
    GameObject* obj, int16 w, int16 h);

// Destroy stage
void destroyStage(Stage* s);

#endif // __STAGE__
