// Stage
// (c) 2019 Jani Nykänen

#include "stage.h"

#include <stdlib.h>
#include <stdio.h>

#define PATH_LEN 16

// Create stage. Returns a pointer
// so that tilemap does not get
// lost
Stage* createStage(uint8 index, Bitmap* bmpTileset) {

    //char path[PATH_LEN];
    uint8 i;

    // Allocate memory
    Stage* s = (Stage*)malloc(sizeof(Stage));
    if(s == NULL) {

        printf("Memory allocation error!\n");
        return NULL;
    }

    // Load tilemap
    //snprintf(path, PATH_LEN, "ASSETS/MAPS/%d.BIN", (int16)index);
    s->tmap = loadTilemap("ASSETS/MAPS/1.BIN");
    if(s->tmap == NULL) {

        free(s);
        return NULL;
    }

    // Allocate memory for the update buffer
    s->updateBuffer = (bool*)malloc(sizeof(bool) 
        * s->tmap->width * s->tmap->height);
    if(s->updateBuffer == NULL) {

        printf("Memory allocation error!\n");
        destroyTilemap(s->tmap);
        free(s);
        return NULL;
    }
    // Set defaults
    for(i = 0; i < s->tmap->width*s->tmap->height; ++ i) {

        s->updateBuffer[i] = false;
    }

    // Store bitmap
    s->bmpTileset = bmpTileset;

    return s;
}


// Update stage
void stageUpdate(Stage* s, int16 steps) {

    // ...
}


// Draw stage
void stageDraw(Stage* s, Graphics* g) {

    int16 x, y;
    int16 sx, sy;
    uint8 t;

    // Draw tiles
    for(y = 0; y < s->tmap->height; ++ y) {

        for(x = 0; x < s->tmap->width; ++ x) {

            if(s->updateBuffer[y*s->tmap->width+ x])
                continue;

            // Update update buffer
            s->updateBuffer[y*s->tmap->width+ x] = true;
            // Check if we want to draw a tile or background
            t = mapGetTile(s->tmap, 0, x, y);
            if(t == 0) {

                gFillRect(g, x*16, y*16, 16, 16, 219);
                continue;
            }
            -- t;

            // Draw tile
            sx = t % 16;
            sx *= 16;
            sy = t / 16;
            sy *= 16;
            gDrawBitmapRegionFast(g, s->bmpTileset, sx, sy, 16, 16, x*16, y*16);
        }
    }
}


// Refresh neighborhood to determine redrawable
// tiles
void stageRefreshNeighborhood(Stage* s, 
    GameObject* obj, int16 w, int16 h) {

    int16 x, y;
    int16 sx, sy;
    int16 ex, ey;
    int16 dx = s->tmap->width*16 / 2;

    // Start position
    // TODO: Pass center point?
    sx = (dx + obj->pos.x/FIXED_PREC) / 16 - w;
    if(sx < 0) sx = 0;
    sy = (obj->pos.y/FIXED_PREC -8) / 16 - h;
    if(sy < 0) sy = 0;

    // Out of range
    if(sx >= s->tmap->width || sy >= s->tmap->height)
        return;

    // End position
    ex = sx + w*2;
    ey = sy + h*2;

    for(y = sy; y <= ey; ++ y) {

        for(x = sx; x <= ex; ++ x) {

            if(x >= s->tmap->width)
                break;

            s->updateBuffer[y*s->tmap->width + x] = false;
        }

        if(y >= s->tmap->height)
            break;
    }

}

// Destroy stage
void destroyStage(Stage* s) {

    if(s == NULL) return;

    destroyTilemap(s->tmap);
    free(s);
}
