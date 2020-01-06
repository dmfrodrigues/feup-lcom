#ifndef NOTHING_H_INCLUDED
#define NOTHING_H_INCLUDED

#include "nothing.xpm"
#include "graph.h"

static basic_sprite_t* get_nothing(void){
    return basic_sprite_ctor((const char *const *)nothing_xpm, 34, 34);
}

#endif //NOTHING_H_INCLUDED
