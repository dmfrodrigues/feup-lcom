#ifndef SHOOTER_H_INCLUDED
#define SHOOTER_H_INCLUDED

#include "shooter.xpm"
#include "graph.h"

static basic_sprite_t* get_shooter(void){
    return basic_sprite_ctor((const char *const *)shooter_xpm, 34, 34);
}

#endif //SHOOTER_H_INCLUDED
