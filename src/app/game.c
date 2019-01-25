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


// Draw a test triangle
static void drawTestTriangle(Graphics* g) {

    Vector2 A, B, C;

    A.x = (fixedCos(angle) * triRadius) / FIXED_PREC;
    A.y = (fixedSin(angle) * triRadius) / FIXED_PREC;

    B.x = (fixedCos(angle+120) * triRadius) / FIXED_PREC;
    B.y = (fixedSin(angle+120) * triRadius) / FIXED_PREC;

    C.x = (fixedCos(angle+240) * triRadius) / FIXED_PREC;
    C.y = (fixedSin(angle+240) * triRadius) / FIXED_PREC;

    drawLine(g, 160+A.x, 100+A.y, 160+B.x, 100+B.y, 255);
    drawLine(g, 160+C.x, 100+C.y, 160+B.x, 100+B.y, 255);
    drawLine(g, 160+A.x, 100+A.y, 160+C.x, 100+C.y, 255);
}


// Initialize
static void gameInit() {

    // Set defaults
    angle = 0;
}


// Update
static void gameUpdate(EventManager* evMan, int16 steps) {

    const DELTA = 8;

    // Check arrow keys
    if(inputGetButton(evMan->input, ArrowLeft) == Down) {

        angle -= 2*steps;
    }
    else if(inputGetButton(evMan->input, ArrowRight) == Down) {

        angle += 2*steps;
    }
    angle = negMod(angle, 360);

    // Scale
    if(inputGetButton(evMan->input, ButtonFire1) == Pressed) {

        triRadius += DELTA;
    }
    else if(inputGetButton(evMan->input, ButtonFire2) == Pressed) {

        triRadius -= DELTA;
    }
}


// Draw
static void gameDraw(Graphics* g) {

    // Draw a test triangle
    fillRect(g, 
        160-triRadius-1, 
        100-triRadius-1,
        triRadius*2  +2,
        triRadius*2  +2, 
        0);
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
