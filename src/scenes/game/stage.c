// Stage
// (c) 2019 Jani Nykänen

#include "stage.h"

#include "../../util/mathext.h"

#include <stdlib.h>
#include <stdio.h>

#include "objman.h"
#include "game.h"

#define PATH_LEN 16


// Draw water tile
static void stageDrawWater(Stage* s, Graphics* g, int16 tx, int16 ty, int16 x, int16 y) {

    const int16 AMPLITUDE = 2;

    int16 sx1 = s->waterPos / FIXED_PREC;
    int16 sw1 = 16-sx1;
    int16 sx2 = 0;
    int16 sw2 = 16-sw1;
    int16 sy = AMPLITUDE-(fixedSin(s->waveTimer) * AMPLITUDE) / FIXED_PREC;
    int16 sh = 16-sy;

    // Fill remaining area
    if(sy > 0) {

        gFillRect(g, x, y, 16, 16-sy, 219);
    }

    // "Left"
    gDrawBitmapRegionFast(g, s->bmpTileset, 
        tx*16 + sx1, ty*16, sw1, sh, x, y+sy);
    // "Right" aka remainder
    gDrawBitmapRegionFast(g, s->bmpTileset, 
        tx*16 + sx2, ty*16, sw2, sh, x + sw1, y+sy);
}



// Create stage. Returns a pointer
// so that tilemap does not get
// lost
Stage* createStage(uint8 index, Bitmap* bmpTileset) {

    //char path[PATH_LEN];
    uint8 i;
    int16 w = VIEW_WIDTH / 16;

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
        * w * s->tmap->height);
    if(s->updateBuffer == NULL) {

        printf("Memory allocation error!\n");
        destroyTilemap(s->tmap);
        free(s);
        return NULL;
    }
    // Set to false
    for(i = 0; i < w*s->tmap->height; ++ i) {

        s->updateBuffer[i] = false;
    }

    // Store bitmap
    s->bmpTileset = bmpTileset;

    // Set defaults
    s->waterPos = 0;
    s->waveTimer = 0;
    s->roomIndex = 0;
    s->forceRedraw = false;
    s->transiting = false;
    s->transTimer = 0;

    return s;
}


// Update stage
void stageUpdate(Stage* s, int16 steps) {

    const int16 WATER_SPEED = 16;
    const int16 WAVE_SPEED = 3;

    // Update transition
    if(s->transiting) {

        s->transTimer += TRANS_SPEED * steps;
        if(s->transTimer >= VIEW_WIDTH) {

            s->transTimer = 0;
            s->transiting = false;
            stageForceRedraw(s);

            // Next room
            ++ s->roomIndex;

            // Redraw info
            gameRefreshInfo();
        }
    }
    else {

        // Update water
        s->waterPos += WATER_SPEED* steps;
        if(s->waterPos >= 16*FIXED_PREC)
            s->waterPos -= 16*FIXED_PREC;

        // Update waves
        s->waveTimer += WAVE_SPEED * steps;
        if(s->waveTimer >= 360)
            s->waveTimer -= 360;
    }
}


// Draw stage
void stageDraw(Stage* s, Graphics* g) {

    int16 x, y;
    int16 sx, sy;
    int16 w = VIEW_WIDTH / 16;
    int16 trx = w*s->roomIndex;
    uint8 t;

    // We use different rendering method for
    // a transiting stage
    if(s->transiting) {

        stageDrawTransition(s, g);
        return;
    }

    // Disable clipping for waster rendering
    gToggleClipping(g, false);

    // Draw tiles
    for(y = 0; y < s->tmap->height; ++ y) {

        for(x = 0; x < w; ++ x) {

            // Get tile
            t = mapGetTile(s->tmap, 0, trx+x, y);

            // If water, use own routine
            if(t > 0 && mapGetTile(s->tmap, 1, trx+x, y) == 129+16) {

                stageDrawWater(s, g, 
                    (t-1) % 16, (t-1) / 16,
                    x*16, y*16);
                continue;
            }

            if(!s->forceRedraw && s->updateBuffer[y*w+ x])
                continue;

            // Update update buffer
            if(!s->forceRedraw)
                s->updateBuffer[y*w+ x] = true;

            // Check if we want to draw a tile or background
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
    s->forceRedraw = false;

    // Reset clipping
    gToggleClipping(g, true);
}


// Draw stage transition
void stageDrawTransition(Stage* s, Graphics* g) {

    int16 x, y;
    int16 sx, sy;
    int16 w = VIEW_WIDTH / 16;
    int16 trx = w*s->roomIndex + s->transTimer;
    uint8 t;

    int16 startx = s->roomIndex*w + (s->transTimer/16);
    int16 endx = startx + w + 1;

    // Toggle clipping
    gToggleClipping(g, true);

    // Draw tiles
    for(y = 0; y < s->tmap->height; ++ y) {

        for(x = startx; x < endx; ++ x) {

            // Get tile
            t = mapGetTile(s->tmap, 0, x, y);

            // Check if we want to draw a tile or background
            if(t == 0) {

                gFillRect(g, x*16-trx, y*16, 16, 16, 219);
                continue;
            }
            -- t;

            // Draw tile
            sx = t % 16;
            sx *= 16;
            sy = t / 16;
            sy *= 16;
            gDrawBitmapRegionFast(g, s->bmpTileset, 
                sx, sy, 16, 16, x*16-trx, y*16);
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
    int16 dx = VIEW_WIDTH/ 2;
    int16 sw = VIEW_WIDTH / 16;

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

            s->updateBuffer[y*sw + x] = false;
        }

        if(y >= s->tmap->height)
            break;
    }

}


// Object collision
void stageCollision(Stage* s, GameObject* obj, int16 steps) {

    const int16 RADIUS = 2;
    const int16 HURT_RADIUS = 10;
    const int16 WATER_OFF = 4;
    
    int16 x, y;
    int16 sx, sy;
    int16 ex, ey;
    uint8 colID;
    int16 w = VIEW_WIDTH / 16;
    int16 trx = w*s->roomIndex;
    int16 dx = w*16 / 2;
    int16 hurtJump = 16 - HURT_RADIUS;

    // Start position
    // TODO: Pass center point?
    sx = (dx + obj->pos.x/FIXED_PREC) / 16 - RADIUS;
    if(sx < 0) sx = 0;
    sy = (obj->pos.y/FIXED_PREC -8) / 16 - RADIUS;
    if(sy < 0) sy = 0;

    // Out of range
    if(sx >= s->tmap->width || sy >= s->tmap->height)
        return;

    // End position
    ex = sx + RADIUS*2;
    ey = sy + RADIUS*2;

    // Go through tiles
    for(y = sy; y <= ey; ++ y) {

        for(x = sx; x <= ex; ++ x) {

            if(x >= s->tmap->width)
                break;

            // Get collision tile
            colID = mapGetTile(s->tmap, 1, trx+x, y);
            if(colID == 0)
                continue;
            colID -= 128;

            // Hurt collision
            if(colID == 16) {

                gobjHurtCollision(obj, x*16-dx+hurtJump, y*16+hurtJump, 
                    HURT_RADIUS, HURT_RADIUS);
            }
            // Water collision
            else if(colID == 17 || colID == 18) {

                gobjWaterCollision(obj, 
                    x*16-dx, y*16 + WATER_OFF, 
                    16, 16-WATER_OFF);
            }
            else {

                // Check base collision
                // 1) Floor
                if(colID == 1 || colID == 5 || colID == 7 || colID == 8 ||
                colID == 11 || colID == 12 || colID == 14 || colID == 15 ) {

                    gobjFloorCollision(obj, x*16-dx, y*16, 16, steps);
                }
                // 2) Wall, right
                if(colID == 4 || colID == 6 || colID == 7 || colID == 10 ||
                colID == 11 || colID == 13 || colID == 14 || colID == 15 ) {

                    gobjWallCollision(obj, x*16-dx, y*16, 16, 1, steps);
                }
                // 3) Wall, left
                if(colID == 2 || colID == 6 || colID == 8 || colID == 9 ||
                colID == 11 || colID == 12 || colID == 13 || colID == 15 ) {

                    gobjWallCollision(obj, (x+1)*16-dx, y*16, 16, -1, steps);
                }
                // 4) Ceiling
                if(colID == 3 || colID == 5 || colID == 9 || colID == 10 ||
                colID == 12 || colID == 13 || colID == 14 || colID == 15 ) {

                    gobjCeilingCollision(obj, x*16-dx, (y+1)*16, 16, steps);
                }
            }
        }

        if(y >= s->tmap->height)
            break;
    }
}


// Parse objects
void stageParseObjects(Stage* s, void* p) {

    int16 x, y;
    int16 tileID;
    int16 w = VIEW_WIDTH / 16;
    int16 dx = w*16 / 2;
    int16 trx = w*s->roomIndex;

    ObjectManager* oman = (ObjectManager*)p;

    for(y = 0; y < s->tmap->height; ++ y) {

        for(x = 0; x < w; ++ x) {

            tileID = mapGetTile(s->tmap, 2, x+trx, y);
            if(tileID == 0)
                continue;
            tileID -= 161;

            switch (tileID)
            {
                // Player
                case 0:

                    if(s->roomIndex == 0)
                        oman->player = plCreate((x+1)*16-dx, (y+1)*16);

                    else {

                        plSetStartPos(&oman->player, (x+1)*16-dx, (y+1)*16);
                    }

                    break;

                // Gem
                case 1:
                    objmanAddGem(oman, x*16 - dx, y*16);
                    break;
            
                default:
                    break;
            }
        }
    }

}


// Force redraw
void stageForceRedraw(Stage* s) {

    s->forceRedraw = true;
}


// Set transition
void stageSetTransition(Stage* s) {

    s->transiting = true;
    s->transTimer = 0;
}


// Destroy stage
void destroyStage(Stage* s) {

    if(s == NULL) return;

    destroyTilemap(s->tmap);
    free(s);
}
