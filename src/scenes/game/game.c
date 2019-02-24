// Game scene
// (c) 2019 Jani Nykänen

#include "game.h"

#include "objman.h"
#include "stage.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../util/mathext.h"
#include "../../core/resources.h"
#include "../../core/tilemap.h"

// Game resources
static ResourceList* res;
// Bitmaps
static Bitmap* bmpFont;

// Object manager
static ObjectManager objm;
// Stage
static Stage* stage;

// Whether the frame has to be
// refreshed
static bool refreshFrame;
// Refresh info
static bool refreshInfo;


// Draw info
static void gameDrawInfo(Graphics* g) {

    const int16 LIVES_X = 48;
    const int16 GEMS_X = 160;
    const int16 STAGE_X = 320-LIVES_X;
    const int16 TEXT_Y = VIEW_HEIGHT + 14;
    const int16 YOFF = 12;

    char gemStr [PL_GEM_MAX +1];
    char lifeStr [PL_LIFE_MAX +1];
    int16 i;

    if(!refreshInfo) return;
    refreshInfo = false;

    // Determine gem string
    for(i = 0; i < PL_GEM_MAX; ++ i) {

        gemStr[i] = (i < objm.player.gems) ? 2 : 1;
    }
    gemStr[i] = '\0';

    // Determine life string
    for(i = 0; i < PL_LIFE_MAX; ++ i) {

        lifeStr[i] = (i < objm.player.lives) ? 4 : 3;
    }
    lifeStr[i] = '\0';

    // Draw lives
    gDrawTextFast(g, bmpFont, "LIVES", LIVES_X, TEXT_Y, 0, 0, true);
    gDrawTextFast(g, bmpFont, lifeStr, LIVES_X, TEXT_Y+YOFF, 2, 0, true);

    // Draw gems
    gDrawTextFast(g, bmpFont, "GEMS", GEMS_X, TEXT_Y, 0, 0, true);
    gDrawTextFast(g, bmpFont, gemStr, GEMS_X, TEXT_Y+YOFF, 2, 0, true);

    // Draw stage
    gDrawTextFast(g, bmpFont, "STAGE", STAGE_X, TEXT_Y, 0, 0, true);
    gDrawTextFast(g, bmpFont, "1-1", STAGE_X, TEXT_Y+YOFF, 0, 0, true);

}


// Draw frame (= the stuff around the viewport, really)
static void gameDrawFrame(Graphics* g) {

    int16 x = 160 - VIEW_WIDTH/2;
    int16 y = 8;

    // Don't redrawn if not necessary
    if(!refreshFrame) return;
    refreshFrame = false;

    // Clear to blue
    gClearScreen(g, 47);

    // Frame
    gDrawRect(g, x-1, y-1, VIEW_WIDTH+2, VIEW_HEIGHT+2, 0);
    gDrawRect(g, x-2, y-2, VIEW_WIDTH+4, VIEW_HEIGHT+4, 255);
    gDrawRect(g, x-3, y-3, VIEW_WIDTH+6, VIEW_HEIGHT+6, 119);
}


// Initialize
static int16 gameInit() {

    bool cond;

    // Create an empty resource list
    res = createEmptyResourceList();
    if(res == NULL) {

        return 1;
    }
    // Load resources
    cond = rsAddBitmap(res, "ASSETS/BITMAPS/FONT.BIN", "font")
        && rsAddBitmap(res, "ASSETS/BITMAPS/RAT.BIN", "rat")
        && rsAddBitmap(res, "ASSETS/BITMAPS/TILESET.BIN", "tileset")
        && rsAddBitmap(res, "ASSETS/BITMAPS/GEM.BIN", "gem");
    if(!cond) {

        return 1;
    }

    // Create stage
    stage = createStage(1, (Bitmap*)rsGetResource(res, "tileset"));
    if(stage == NULL) {

        return 1;
    }

    // Get bitmaps
    bmpFont = (Bitmap*)rsGetResource(res, "font");

    // Initialize global content
    objmanInit(res);
    // Create components
    objm = objmanCreate(stage);

    // Set defaults
    refreshFrame = true;
    refreshInfo = true;

    return 0;
}


// Update
static void gameUpdate(EventManager* evMan, int16 steps) {

    Vpad* vpad = evMan->vpad;

    // Update stage
    stageUpdate(stage, steps);

    // Update objects
    objmanUpdate(&objm, evMan, stage, steps);
    
    // Escape (TEMP!)
    if(vpad->buttons[4].state == Pressed) {

        eventTerminate(evMan);
    }
}


// Draw
static void gameDraw(Graphics* g) {

    // Reset view
    gTranslate(g, 0, 0);
    gResetViewport(g);
    // Draw frame
    gameDrawFrame(g);
    // Draw info
    gameDrawInfo(g);

    // Set viewport
    gSetViewport(g, 8, 8, VIEW_WIDTH, VIEW_HEIGHT);

    // Draw stage
    gTranslate(g, 8, 8);
    stageDraw(stage, g);

    // Draw objects
    objmanDraw(&objm, g);
}


// Dispose
static void gameDispose() {

    // Destroy content
    destroyResList(res);
}


// Refresh info
void gameRefreshInfo() {

    refreshInfo = true;
}


// Get game scene
Scene getGameScene() {

    Scene s;
    s.init = gameInit;
    s.update = gameUpdate;
    s.draw = gameDraw;
    s.dispose = gameDispose;
    snprintf(s.name, SCENE_NAME_LENGTH, "game");

    return s;
}
