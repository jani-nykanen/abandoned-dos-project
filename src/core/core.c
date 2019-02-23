// Application core
// (c) 2019 Jani Nykänen

#include "core.h"

#include "err.h"

#include <dos.h>
#include <conio.h>
#include <bios.h>

#include <stdlib.h>
#include <stdio.h>


// Vertical sync
static void vsync() {

    while(inp(0x3DA) & 8);
    while(!(inp(0x3DA) & 8));
}


// Initialize scenes
static int16 coreInitScenes(Core* c) {

    uint8 i;
    for(i=0; i < c->sceneCount; ++ i) {

        if(c->scenes[i]->init != NULL) {

            if(c->scenes[i]->init() != 0)
                return 1;
        }
    }
    return 0;
}


// Dispose
static void coreDispose(Core* c) {

    uint8 i;

    // Dispose scenes
    for(i = 0; i < c->sceneCount; ++ i) {

        if(c->scenes[i]->dispose != NULL) {

            c->scenes[i]->dispose();
        }
    }

    // Destroy components
    destroyGraphics(c->g);
    destroyInputManager(c->input);
}


// Initialize graphics
static int16 coreInitGraphics(Core* c) {

    // Create a graphics object
    c->g = createGraphics();
    return c->g != NULL ? 0 : 1;
}


// Create a new application core
void createAppCore(Core* c) {

    // Set defaults
    c->sceneCount = 0;
    c->activeScene = NULL;
    c->frameSkip = 1;
    c->stepCount = 0;
    c->running = true;

    // Initialize error handling
    errInit();

    // Create an input manager
    c->input = createInputManager();
    // Create vpad
    c->vpad = createVpad();
    // Create an event manager
    c->evMan = createEventManager(
        (void*)c, c->input, &c->vpad);
}


// Add a scene
void coreAddScene(Core* c, Scene* s, bool makeActive) {

    if(c->sceneCount >= MAX_SCENES) return;

    c->scenes[c->sceneCount ++] = s;
    if(makeActive)
        c->activeScene = s;
}


// Enter the main loop
void coreLoop(Core*c) {

    bool updateFrame =false;

    // Initialize scenes before looping
    if(coreInitScenes(c) != 0) {

        c->running = false;
    }

    // Initialize graphics
    if(coreInitGraphics(c) != 0) {

        printf("Fatal error: %s\n", errGet());
        exit(1);
    }

    // Start the main loop
    c->stepCount = 0;
    while(c->running) {

        
        // Check frame skipping
        updateFrame = false;
        if(c->frameSkip == 0 || 
           ++ c->stepCount > c->frameSkip) {

            c->stepCount = 0;
            updateFrame = true;
        }

        // Update & render the active scene
        if(updateFrame 
            && c->activeScene != NULL) {

            // Update virtual gamepad
            vpadUpdate(&c->vpad, c->input);
            // Update
            if(c->activeScene->update != NULL) {

                c->activeScene->update(&c->evMan, 
                    c->frameSkip +1);
            }

            // Draw
            if(c->activeScene->draw != NULL) {

                c->activeScene->draw(c->g);
            }
        }

        // Wait for the vertical sync
        vsync();

        // Draw frame
        gDrawFrame(c->g);
    }

    // Dispose
    coreDispose(c);

    // Write error, if any
    if(errGet() != NULL) {

        printf("Fatal error: %s\n", errGet());
    }
}


// Initialize virtual gamepad
void coreInitVpad(Core* c, void (*cb)(Vpad*)) {

    if(cb != NULL)
        cb(&c->vpad);
}


// Terminate
void coreTerminate(Core* c) {

    // Stop
    c->running = false;
}
