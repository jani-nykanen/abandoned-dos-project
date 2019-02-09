// Scene
// (c) 2019 Jani Nykänen

#ifndef __SCENE_H__
#define __SCENE_H__

#include "types.h"
#include "graphics.h"
#include "eventman.h"

#define SCENE_NAME_LENGTH 32

// Scene type
typedef struct {

    // Methods
    int16 (*init)    (void);
    void  (*update)  (EventManager*, int16);
    void  (*draw)    (Graphics*);
    void  (*dispose) (void);

    // Name
    char name [SCENE_NAME_LENGTH];

} Scene;

#endif // __SCENE_H__
