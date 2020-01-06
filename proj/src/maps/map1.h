#ifndef MAP1_H_INCLUDED
#define MAP1_H_INCLUDED

#include "libs.h"

static map_t* get_map1(void){
    char **ret         = xpm_load_xpm2("/home/lcom/labs/proj/src/maps/xpm2/map1.xpm2");
    char **ret_collide = xpm_load_xpm2("/home/lcom/labs/proj/src/maps/xpm2/map1_collide.xpm2");
    map_t *r = NULL;
    if(ret != NULL && ret_collide != NULL){
        r = map_ctor((const char *const *)ret, (const char *const *)ret_collide);
    }else{
        if(ret         == NULL) printf("%s: map is NULL\n", __func__);
        if(ret_collide == NULL) printf("%s: collide is NULL\n", __func__);
    }
    free(ret); free(ret_collide);
    return r;
}

#endif /* end of include guard: MAP_H_INCLUDED */
