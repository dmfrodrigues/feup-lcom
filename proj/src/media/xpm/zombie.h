#ifndef ZOMBIE_H_INCLUDED
#define ZOMBIE_H_INCLUDED

#include "zombie.xpm"
#include "graph.h"

static basic_sprite_t* get_zombie(void){
    return basic_sprite_ctor((const char *const *)zombie_xpm, 34, 34);
}

#endif //ZOMBIE_H_INCLUDED
