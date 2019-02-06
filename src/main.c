// Main
// (c) 2019 Jani Nykänen

#include "core/core.h"

#include "app/game.h"


// Main function
void main() {

    Core c;
    Scene game;

    // Create application
    createAppCore(&c);
    
    // Add scenes
    game = getGameScene();
    coreAddScene(&c, &game, true);

    // Loop
    coreLoop(&c);
}
