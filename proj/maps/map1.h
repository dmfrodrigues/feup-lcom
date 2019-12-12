#ifndef MAP1_H_INCLUDED
#define MAP1_H_INCLUDED

#include "map1.xpm"
#include "map1_collide.xpm"
#include "sprite.h"

map_t* get_map1(void){
    return map_ctor((const char **)map1_xpm, (const char **)map1_collide_xpm);
}

#endif /* end of include guard: MAP_H_INCLUDED */
