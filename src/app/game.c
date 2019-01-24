// Game scene
// (c) 2019 Jani Nykänen

#include "game.h"

#include <string.h>
#include <stdio.h>

#include "../util/mathext.h"

// Triangle angle
static int16 angle;


// Draw a test triangle
static void drawTestTriangle(Graphics* g) {

    const int RADIUS = 64;

    Vector2 A, B, C;

    A.x = (fixedCos(angle) * RADIUS) / FIXED_PREC;
    A.y = (fixedSin(angle) * RADIUS) / FIXED_PREC;

    B.x = (fixedCos(angle+120) * RADIUS) / FIXED_PREC;
    B.y = (fixedSin(angle+120) * RADIUS) / FIXED_PREC;

    C.x = (fixedCos(angle+240) * RADIUS) / FIXED_PREC;
    C.y = (fixedSin(angle+240) * RADIUS) / FIXED_PREC;

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
static void gameUpdate(int16 steps) {

    angle += 2*steps;
    if(angle >= 360)
        angle -= 360;
}


// Draw
static void gameDraw(Graphics* g) {

    // Draw a test triangle
    fillRect(g, 92,36,136,128, 0);
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
