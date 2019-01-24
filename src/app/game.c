// Game scene
// (c) 2019 Jani Nykänen

#include "game.h"

#include <string.h>
#include <stdio.h>


// Initialize
static void gameInit() {

    // ...
}


// Update
static void gameUpdate(int16 steps) {

}


// Draw
static void gameDraw(Graphics* g) {

    clearScreen(g, 0);

    // Draw a white line
    drawLine(g, 32, 32, 128, 64, 255);
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
