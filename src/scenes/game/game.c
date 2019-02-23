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
static Bitmap* bmpPlayer;

// Object manager
static ObjectManager objm;
// Stage
static Stage* stage;

// Whether the frame has to be
// refreshed
static bool refreshFrame;


// Draw frame (= the stuff around the viewport, really)
static void gameDrawFrame(Graphics* g) {

    int16 x = 160 - VIEW_WIDTH/2;
    int16 y = 8;

    // Don't redrawn if not necessary
    if(!refreshFrame) return;
    refreshFrame = false;

    // Reset view
    gResetViewport(g);
    gTranslate(g, 0, 0);

    // Clear to black
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
        && rsAddBitmap(res, "ASSETS/BITMAPS/TILESET.BIN", "tileset");
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
    bmpPlayer = (Bitmap*)rsGetResource(res, "player");

    // Initialize global content
    objmanInit(res);

    // Create components
    objm = objmanCreate();

    // Set defaults
    refreshFrame = true;

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
