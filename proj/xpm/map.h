#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "map.xpm"
#include "sprite.h"

basic_sprite_t* get_map(void){
    return basic_sprite_ctor((const char **)map_xpm, 0, 0);
}

#endif /* end of include guard: MAP_H_INCLUDED */
