// Game scene
// (c) 2019 Jani Nykänen

#include "game.h"

#include <string.h>
#include <stdio.h>

#include "../util/mathext.h"

// Triangle angle
static int16 angle;
// Triangle radius
static int16 triRadius = 64;
static int16 oldRadius = 64;

// Triangle position
static Vector2 trianglePos;


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
static void gameInit() {

    // Set defaults
    angle = 0;

    trianglePos.x = 132;
    trianglePos.y = 100;
}


// Update
static void gameUpdate(EventManager* evMan, int16 steps) {

    const int16 DELTA = 1;

    // Check arrow keys
    if(inputGetArrow(evMan->input, ArrowLeft) == Down) {

        angle -= 2*steps;
        trianglePos.x -= 1*steps;
    }
    else if(inputGetArrow(evMan->input, ArrowRight) == Down) {

        angle += 2*steps;
        trianglePos.x += 1*steps;
    }
    angle = negMod(angle, 360);

    // Scale
    oldRadius = triRadius;
    if(inputGetKey(evMan->input, 44) == Down) {

        triRadius += DELTA *steps;
    }
    else if(inputGetKey(evMan->input, 45) == Down) {

        triRadius -= DELTA *steps;
    }

    // Escape (TEMP!)
    if(inputGetKey(evMan->input, 1) == Pressed) {

        eventTerminate(evMan);
    }
}


// Draw
static void gameDraw(Graphics* g) {

    gResetViewport(g);
    gClearScreen(g, 0);

    // Set viewport
    gSetViewport(g, 4, 4, 224, 192);
    gClearView(g, 3);

    // Draw rectangle inside a triangle
    gFillRect(g, trianglePos.x-triRadius/2, 
                 trianglePos.y-triRadius/2,
                 triRadius, triRadius, 224);

    // Draw a test triangle
    drawTestTriangle(g);
}


// Dispose
static void gameDispose() {

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
