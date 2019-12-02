#include "bullet.xpm"
#include "sprite.h"

basic_sprite_t* get_bullet(void){
    return basic_sprite_ctor((const char **)bullet_xpm, 3, 7);
}
