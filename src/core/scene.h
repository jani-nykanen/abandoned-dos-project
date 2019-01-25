// Scene
// (c) 2019 Jani Nykänen

#ifndef __SCENE_H__
#define __SCENE_H__

#include "types.h"
#include "graphics.h"
#include "eventman.h"

#define SCENE_NAME_LENGTH 32

// Types
typedef char NameString[SCENE_NAME_LENGTH];
typedef void (*Function) (void);
typedef void (*FunctionParamEventManInt) (EventManager*, int16);
typedef void (*FunctionParamGraph) (Graphics*g);

// Scene type
typedef struct {

    // Methods
    Function                   init;
    FunctionParamEventManInt   update;
    FunctionParamGraph         draw;
    Function                   dispose;

    // Name
    NameString name;

} Scene;

#endif // __SCENE_H__
