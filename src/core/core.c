// Application core
// (c) 2019 Jani Nykänen

#include "core.h"

#include <dos.h>
#include <conio.h>
#include <bios.h>

#include <stdlib.h>


// Vertical sync
static void vsync() {

    while(inp(0x3DA) & 8);
    while(!(inp(0x3DA) & 8));
}


// Dispose
static void dispose(Core* c) {

    uint8 i;

    // Dispose scenes
    for(i = 0; i < c->sceneCount; ++ i) {

        if(c->scenes[i]->dispose != NULL) {

            c->scenes[i]->dispose();
        }
    }

    // Destroy components
    free(c->g);
}


// Create a new application core
Core createAppCore() {

    Core c;

    // Set defaults
    c.sceneCount = 0;
    c.activeScene = NULL;
    c.frameSkip = 1;
    c.stepCount = 0;

    // Create graphics
    c.g = createGraphics();

    return c;   
}


// Add a scene
void coreAddScene(Core*c, Scene* s) {

    if(c->sceneCount >= MAX_SCENES) return;

    c->scenes[c->sceneCount ++] = s;
}


// Enter the main loop
void coreLoop(Core*c) {

    bool updateFrame =false;

    c->stepCount = 0;
    while(c->running) {

        // Wait for the vertical sync
        vsync();

        // Check frame skipping
        updateFrame = true;
        if(c->frameSkip > 0 
           && ++ c->stepCount > c->frameSkip) {

            c->stepCount = 0;
            updateFrame = false;
        }

        // Update & render the active scene
        if(updateFrame && c->activeScene != NULL) {

            // Update
            if(c->activeScene->update != NULL) {

                c->activeScene->update(c->stepCount +1);
            }

            // Draw
            if(c->activeScene->draw != NULL) {

                c->activeScene->draw(c->g);
            }
        }
    }

    // Dispose
    dispose(c);
}


// Terminate
void coreTerminate(Core* c) {

    // Stop
    c->running = false;
}
