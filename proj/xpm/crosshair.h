#include "crosshair.xpm"
#include "sprite.h"

sprite_t* get_crosshair(){
    return sprite_ctor(crosshair_xpm, 16, 16);
}
