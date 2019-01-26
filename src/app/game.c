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

    drawLine(g, trianglePos.x+A.x, trianglePos.y+A.y, trianglePos.x+B.x, trianglePos.y+B.y, 255);
    drawLine(g, trianglePos.x+C.x, trianglePos.y+C.y, trianglePos.x+B.x, trianglePos.y+B.y, 255);
    drawLine(g, trianglePos.x+A.x, trianglePos.y+A.y, trianglePos.x+C.x, trianglePos.y+C.y, 255);
}


// Initialize
static void gameInit() {

    // Set defaults
    angle = 0;

    trianglePos.x = 64;
    trianglePos.y = 100;
}


// Update
static void gameUpdate(EventManager* evMan, int16 steps) {

    const int16 DELTA = 1;

    // Check arrow keys
    if(inputGetButton(evMan->input, ArrowLeft) == Down) {

        angle -= 2*steps;
    }
    else if(inputGetButton(evMan->input, ArrowRight) == Down) {

        angle += 2*steps;
    }
    angle = negMod(angle, 360);

    // Scale
    oldRadius = triRadius;
    if(inputGetButton(evMan->input, ButtonFire1) == Down) {

        triRadius += DELTA *steps;
    }
    else if(inputGetButton(evMan->input, ButtonFire2) == Down) {

        triRadius -= DELTA *steps;
    }
}


// Draw
static void gameDraw(Graphics* g) {

    clearScreen(g, 0);

    // Draw a test triangle
    /*
    fillRect(g, 
        160-oldRadius-1, 
        100-oldRadius-1,
        oldRadius*2  +2,
        oldRadius*2  +2, 
        0);
        */
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
