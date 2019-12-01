#include "shooter.xpm"
#include "sprite.h"

sprite_t* get_shooter(){
    return sprite_ctor((const char **)shooter_xpm, 34, 34);
}
