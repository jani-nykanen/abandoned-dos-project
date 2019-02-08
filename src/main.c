// Main
// (c) 2019 Jani Nykänen

#include "core/core.h"

#include "scenes/game/game.h"

// Initialize gamepad
static void initVpad(Vpad* vpad) {

    vpadAddButton(vpad, 0, 44); // Z
    vpadAddButton(vpad, 1, 45); // X
    vpadAddButton(vpad, 2, 46); // C
    vpadAddButton(vpad, 3, 0x1C); // Enter
    vpadAddButton(vpad, 4, 1); // Enter
}


// Main function
void main() {

    Core c;
    Scene game;

    // Create application
    createAppCore(&c);
    
    // Add scenes
    game = getGameScene();
    coreAddScene(&c, &game, true);

    // Initialize virtual gamepad
    coreInitVpad(&c, initVpad);

    // Loop
    coreLoop(&c);
}
