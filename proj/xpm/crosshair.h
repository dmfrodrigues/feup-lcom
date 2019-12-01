#include "crosshair.xpm"
#include "sprite.h"

basic_sprite_t* get_crosshair(){
    return basic_sprite_ctor(crosshair_xpm, 16, 16);
}
