// Main
// (c) 2019 Jani Nykänen

#include "core/core.h"

#include "app/game.h"


// Main function
void main() {

    // Create application
    Core c = createAppCore();
    // Add scenes
    Scene game = getGameScene();
    coreAddScene(&c, &game, true);

    // Loop
    coreLoop(&c);
}
