#include "shooter.xpm"
#include "sprite.h"

sprite_t* get_shooter(){
    return sprite_ctor(shooter_xpm, 24, 24);
}
