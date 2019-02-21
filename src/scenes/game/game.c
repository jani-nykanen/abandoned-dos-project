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

// Frame size
static const int16 FRAME_WIDTH = 208;
static const int16 FRAME_HEIGHT = 176;

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
static void drawFrame(Graphics* g) {

    int16 fd = (200 - FRAME_HEIGHT)/2;

    // Don't redrawn if not necessary
    if(!refreshFrame) return;
    refreshFrame = false;

    // Reset view
    gResetViewport(g);
    gTranslate(g, 0, 0);

    // Clear to black
    gClearScreen(g, 0);

    // Frame
    gDrawRect(g, fd-1, fd-1, FRAME_WIDTH+2, FRAME_HEIGHT+2, 0);
    gDrawRect(g, fd-2, fd-2, FRAME_WIDTH+4, FRAME_HEIGHT+4, 255);

    // "Hello world!" & more text
    gDrawTextFast(g, bmpFont, "Hello goat!",320-(320-FRAME_WIDTH-fd)/2, 16, true);

    gDrawTextFast(g, bmpFont, "Life is\nawwwsom!",FRAME_WIDTH+fd*2, 32, false);

    // Set viewport
    gSetViewport(g, fd, fd, FRAME_WIDTH, FRAME_HEIGHT);
    gTranslate(g, fd, fd);
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
        && rsAddBitmap(res, "ASSETS/BITMAPS/PLAYER.BIN", "player")
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

    // Update objects
    objmanUpdate(&objm, evMan, steps);

    // Escape (TEMP!)
    if(vpad->buttons[4].state == Pressed) {

        eventTerminate(evMan);
    }
}


// Draw
static void gameDraw(Graphics* g) {

    gTranslate(g, 0, 0);
    gResetViewport(g);

    // Clear background
    // gClearScreen(g, 111);

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
