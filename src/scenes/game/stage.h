// Stage
// (c) 2019 Jani Nykänen

#ifndef __STAGE__
#define __STAGE__

#include "../../core/tilemap.h"
#include "../../core/graphics.h"

// Stage type
typedef struct {

    // Tilemap
    Tilemap* tmap;
    // Tileset
    Bitmap* bmpTileset;

    // Stage index
    uint8 index;

} Stage;

// Create stage. Returns a pointer
// so that tilemap does not get
// lost
Stage* createStage(uint8 index, Bitmap* bmpTileset);

// Update stage
void stageUpdate(Stage* s, int16 steps);

// Draw stage
void stageDraw(Stage* s, Graphics* g);

// Destroy stage
void destroyStage(Stage* s);

#endif // __STAGE__
