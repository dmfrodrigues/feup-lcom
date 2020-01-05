#ifndef MAP1_H_INCLUDED
#define MAP1_H_INCLUDED

#include "map1.xpm"
#include "map1_collide.xpm"
#include "graph.h"

#include "xpm_utils.h"

static map_t* get_map1(void){
    char **ret         = xpm_load_xpm2("maps/map1.xpm2");
    char **ret_collide = xpm_load_xpm2("maps/map1_collide.xpm2");
    return map_ctor((const char *const *)ret, (const char *const *)ret_collide);
    free(ret); free(ret_collide);
}

#endif /* end of include guard: MAP_H_INCLUDED */
