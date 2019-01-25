// Application core
// (c) 2019 Jani Nykänen

#ifndef __CORE_H__
#define __CORE_H__

#include <stdbool.h>

#include "scene.h"
#include "types.h"
#include "graphics.h"
#include "input.h"

#define MAX_SCENES 16

// Application core
typedef struct {

    // Graphics object
    Graphics* g;

    // Scenes
    Scene* scenes[MAX_SCENES];
    // Scene count
    uint8 sceneCount;
    // Active scene
    Scene* activeScene;

    // Frame skip
    int16 frameSkip;
    // Step count
    int16 stepCount;

    // Input manager
    InputManager* input;
    // Event manager
    EventManager evMan;

    // Is running
    bool running;

} Core;

// Create a new application core
Core createAppCore();

// Add a scene
void coreAddScene(Core*c, Scene* s, bool makeActive);

// Enter the main loop
void coreLoop(Core*c);

// Terminate
void coreTerminate(Core* c);

#endif // __CORE_H__
