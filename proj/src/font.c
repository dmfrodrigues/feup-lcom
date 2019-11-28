#include <lcom/lcf.h>

#include "font.h"

struct glyph{
    uint16_t w, h;
    uint8_t *map;
};

glyph_t* (glyph_ctor)(const char **xpm){
    glyph_t *ret = malloc(sizeof(glyph_t));
    if(ret == NULL) return NULL;
    enum xpm_image_type type = XPM_8_8_8_8;
    xpm_image_t img;
    ret->map = xpm_load((xpm_map_t)xpm, type, &img);
    if(ret->map == NULL){
        free(ret);
        return NULL;
    }
    ret->w = img.width;
    ret->h = img.height;
    return ret;
}

void (glyph_dtor)(glyph_t *p){
    if(p == NULL) return;
    free(p->map);
    free(p);
}
