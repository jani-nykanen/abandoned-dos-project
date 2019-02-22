// A game object base class
// (c) 2019 Jani Nykänen

#include "gameobject.h"


// Floor collision
bool gobjFloorCollision(GameObject* gobj, 
    int16 x, int16 y, int16 w, int16 steps) {

    const int EPS1 = 1;
    const int EPS2 = 2;

    int16 px, py, sy;

    if(gobj->speed.y < 0) 
        return false;

    py = gobj->pos.y/FIXED_PREC;
    px = gobj->pos.x/FIXED_PREC;
    sy = gobj->speed.y/FIXED_PREC;

    // Check if horizontally inside the collision area
    if(px+(gobj->width/2) >= x && px-(gobj->width/2) < x+w) {

        // Check if vertically inside
        if(py >= y-EPS1 && py <= y+(EPS2+sy)*steps) {

            gobj->pos.y = y * FIXED_PREC;
            gobj->speed.y = 0;
            gobj->canJump = true;

            return true;
        }
    }
    
    return false;
}


// Wall collision
bool gobjWallCollision(GameObject* gobj,
    int16 x, int16 y, int16 h, int16 dir, 
    int16 steps) {

    const int EPS1 = 1;
    const int EPS2 = 2;

    int16 px, py, sx;

    if(gobj->speed.x*dir < 0) 
        return false;

    px = gobj->pos.x/FIXED_PREC;
    py = gobj->pos.y/FIXED_PREC;
    
    sx = gobj->speed.x/FIXED_PREC;

    // Check if vertically inside the collision area
    if(py > y && py-gobj->height < y+h) {

        // Check if horizontally inside
        if((dir > 0 && px+gobj->width/2 >= x-EPS1 
            && px+gobj->width/2 <= x + (EPS2+sx)*steps) ||
           (dir < 0 && px-gobj->width/2 <= x+EPS1 
            && px-gobj->width/2 >= x + (-EPS2+sx)*steps)) {

            gobj->pos.x = (x-dir*gobj->width/2) * FIXED_PREC;
            gobj->speed.x = 0;

            return true;
        }
    }

    return false;
}


// Ceiling collision
bool gobjCeilingCollision(GameObject* gobj, 
    int16 x, int16 y, int16 w, int16 steps) {

    const int EPS1 = 1;
    const int EPS2 = 2;

    int16 px, py, sy;

    if(gobj->speed.y > 0) 
        return false;

    py = gobj->pos.y/FIXED_PREC;
    px = gobj->pos.x/FIXED_PREC;
    sy = gobj->speed.y/FIXED_PREC;

    // Check if horizontally inside the collision area
    if(px+(gobj->width/2) >= x && px-(gobj->width/2) < x+w) {

        // Check if vertically inside
        if(py-gobj->height <= y+EPS1 
         && py-gobj->height >= y+(-EPS2+sy)*steps) {

            gobj->pos.y = (y+gobj->height) * FIXED_PREC;
            gobj->speed.y = 0;

            return true;
        }
    }
    
    return false;
}
