#ifndef CROSSHAIR_H_INCLUDED
#define CROSSHAIR_H_INCLUDED

#include "crosshair.xpm"
#include "graph.h"

basic_sprite_t* get_crosshair(){
    return basic_sprite_ctor(crosshair_xpm, 16, 16);
}

#endif //CROSSHAIR_H_INCLUDED
