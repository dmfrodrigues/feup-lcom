#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "bullet.xpm"
#include "graph.h"

static basic_sprite_t* get_bullet(void){
    return basic_sprite_ctor((const char *const *)bullet_xpm, 3, 7);
}

#endif /* end of include guard: BULLET_H_INCLUDED */
