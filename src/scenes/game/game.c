// Game scene
// (c) 2019 Jani Nykänen

#include "game.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../util/mathext.h"
#include "../../core/resources.h"

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


// Initialize
static int16 gameInit() {

    bool cond;

    // Create an empty resource list
    res = createEmptyResourceList();
    if(res == NULL) {

        return 1;
    }
    // Load resources
    cond = rsAddBitmap(res, "ASSETS/BITMAPS/FONT.BIN", "font");
    if(!cond) {

        return 1;
    }

    // Get bitmaps
    bmpFont = (Bitmap*)rsGetResource(res, "font");

    // Set defaults
    angle = 0;
    trianglePos.x = 132;
    trianglePos.y = 100;

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

    gResetViewport(g);
    gClearScreen(g, 0);

    // "Hello world!" & more text
    gDrawTextFast(g, bmpFont, "Hello goat!",320-(320-224-4)/2, 16, true);

    gDrawTextFast(g, bmpFont, "Life is\nawwwsom!",224+8, 32, false);

    // Set viewport
    gSetViewport(g, 4, 4, 224, 192);
    gClearView(g, 3);

    // Draw test bitmap
    gDrawBitmapRegionFast(g, bmpFont, 32,0, 64,64, 
        trianglePos.x-32,
        trianglePos.y-32);

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
