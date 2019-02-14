// Game scene
// (c) 2019 Jani Nykänen

#include "game.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../util/mathext.h"
#include "../../core/resources.h"

// Frame size
static const int16 FRAME_WIDTH = 208;
static const int16 FRAME_HEIGHT = 176;

// Triangle angle
static int16 angle;
// Triangle radius
static int16 triRadius = 64;
static int16 oldRadius = 64;

// Triangle position
static Vector2 trianglePos;

// Game resources
static ResourceList* res;
// Bitmaps
static Bitmap* bmpFont;
static Bitmap* bmpPlayer;

// Whether the frame has to be
// refreshed
static bool refreshFrame;


// Draw a test triangle
static void drawTestTriangle(Graphics* g) {

    Vector2 A, B, C;

    A.x = (fixedCos(angle) * triRadius) / FIXED_PREC;
    A.y = (fixedSin(angle) * triRadius) / FIXED_PREC;

    B.x = (fixedCos(angle+120) * triRadius) / FIXED_PREC;
    B.y = (fixedSin(angle+120) * triRadius) / FIXED_PREC;

    C.x = (fixedCos(angle+240) * triRadius) / FIXED_PREC;
    C.y = (fixedSin(angle+240) * triRadius) / FIXED_PREC;

    gDrawLine(g, trianglePos.x+A.x, trianglePos.y+A.y, trianglePos.x+B.x, trianglePos.y+B.y, 255);
    gDrawLine(g, trianglePos.x+C.x, trianglePos.y+C.y, trianglePos.x+B.x, trianglePos.y+B.y, 255);
    gDrawLine(g, trianglePos.x+A.x, trianglePos.y+A.y, trianglePos.x+C.x, trianglePos.y+C.y, 255);
}


// Draw frame (= the stuff around the viewport, really)
static void drawFrame(Graphics* g) {

    int16 fd = (200 - FRAME_HEIGHT)/2;

    // Don't redrawn if not necessary
    if(!refreshFrame) return;
    refreshFrame = false;

    gResetViewport(g);
    gClearScreen(g, 0);

    // Frame
    gDrawRect(g, fd-1, fd-1, FRAME_WIDTH+2, FRAME_HEIGHT+2, 0);
    gDrawRect(g, fd-2, fd-2, FRAME_WIDTH+4, FRAME_HEIGHT+4, 255);

    // "Hello world!" & more text
    gDrawTextFast(g, bmpFont, "Hello goat!",320-(320-FRAME_WIDTH-fd)/2, 16, true);

    gDrawTextFast(g, bmpFont, "Life is\nawwwsom!",FRAME_WIDTH+fd*2, 32, false);

    // Set viewport
    gSetViewport(g, fd, fd, FRAME_WIDTH, FRAME_HEIGHT);
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
        && rsAddBitmap(res, "ASSETS/BITMAPS/PLAYER.BIN", "player");
    if(!cond) {

        return 1;
    }

    // Get bitmaps
    bmpFont = (Bitmap*)rsGetResource(res, "font");
    bmpPlayer = (Bitmap*)rsGetResource(res, "player");

    // Set defaults
    angle = 0;
    trianglePos.x = 132;
    trianglePos.y = 100;
    refreshFrame = true;

    return 0;
}


// Update
static void gameUpdate(EventManager* evMan, int16 steps) {

    const int16 DELTA = 1;
    Vpad* vpad = evMan->vpad;

    // Check arrow keys
    if(vpad->stick.x < 0) {

        angle -= 2*steps;
        trianglePos.x -= 1*steps;
    }
    else if(vpad->stick.x > 0) {

        angle += 2*steps;
        trianglePos.x += 1*steps;
    }
    if(vpad->stick.y < 0) {

        trianglePos.y -= 1*steps;
    }
    else if(vpad->stick.y > 0) {

        trianglePos.y += 1*steps;
    }
    angle = negMod(angle, 360);

    // Scale
    oldRadius = triRadius;
    if(vpad->buttons[0].state == Down) {

        triRadius += DELTA *steps;
    }
    else if(vpad->buttons[1].state == Down) {

        triRadius -= DELTA *steps;
    }

    // Escape (TEMP!)
    if(vpad->buttons[4].state == Pressed) {

        eventTerminate(evMan);
    }
}


// Draw
static void gameDraw(Graphics* g) {

    // Draw frame
    drawFrame(g);

    gClearView(g, 111);

    // Draw test bitmap
    gDrawBitmap(g, bmpPlayer, 
        trianglePos.x-32,
        trianglePos.y-24, true);

    // Draw a test triangle
    drawTestTriangle(g);
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
