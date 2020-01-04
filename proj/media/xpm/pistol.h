#ifndef PISTOL_H_INCLUDED
#define PISTOL_H_INCLUDED

#include "pistol.xpm"
#include "graph.h"

static basic_sprite_t* get_pistol(void){
    return basic_sprite_ctor((const char *const *)pistol_xpm, 34, 34);
}

#endif //PISTOL_H_INCLUDED
