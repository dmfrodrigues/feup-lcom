#include <lcom/lcf.h>

#include "sprite.h"

#include "graph.h"
#include "utils.h"
#include "fast_math.h"
#include <math.h>

struct basic_sprite{
    uint8_t *map;
    uint16_t w, h;
    int16_t u0, v0;
};

basic_sprite_t* (basic_sprite_ctor)(const char **xpm, int16_t u0, int16_t v0){
    basic_sprite_t *ret = malloc(sizeof(basic_sprite_t));
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
    ret->u0 = u0;
    ret->v0 = v0;
    return ret;
}
void (basic_sprite_dtor)(basic_sprite_t *p){
    if(p == NULL) return;
    free(p->map);
    free(p);
}

const uint8_t* (basic_sprite_get_map)(const basic_sprite_t *p){ return p->map; }
uint16_t       (basic_sprite_get_w)  (const basic_sprite_t *p){ return p->w  ; }
uint16_t       (basic_sprite_get_h)  (const basic_sprite_t *p){ return p->h  ; }
int16_t        (basic_sprite_get_u0) (const basic_sprite_t *p){ return p->u0 ; }
int16_t        (basic_sprite_get_v0) (const basic_sprite_t *p){ return p->v0 ; }

struct sprite{
    const basic_sprite_t *bsp;
    int16_t x, y; //position in screen
    double theta, s, c;
    double scale;
};

sprite_t* (sprite_ctor)(const basic_sprite_t *bsp){
    sprite_t *ret = malloc(sizeof(sprite_t));
    if(ret == NULL) return NULL;
    ret->bsp = bsp;
    ret->x = 0;
    ret->y = 0;
    sprite_set_angle(ret, 0.0);
    ret->scale = 1.0;
    return ret;
}
void (sprite_dtor)(sprite_t *p){
    if(p == NULL) return;
    free(p);
}

void (sprite_set_pos)   (sprite_t *p, int16_t x , int16_t y ){ p->x = x; p->y = y; }
void (sprite_set_angle) (sprite_t *p, double angle          ){ p->theta = angle; p->c = fm_cos(p->theta); p->s = fm_sin(p->theta); }
void (sprite_set_scale) (sprite_t *p, double scale          ){ p->scale = scale; }
int16_t  (sprite_get_x)(const sprite_t *p){ return p->x; }
int16_t  (sprite_get_y)(const sprite_t *p){ return p->y; }

void (sprite_src2pic)(const sprite_t *p, int16_t x, int16_t y, int16_t *u, int16_t *v){
    double dx = (x - p->x)/p->scale;
    double dy = (y - p->y)/p->scale;
    int16_t du = dx*p->c - dy*p->s + 0.5;
    int16_t dv = dx*p->s + dy*p->c + 0.5;
    *u = du + basic_sprite_get_u0(p->bsp);
    *v = dv + basic_sprite_get_v0(p->bsp);
}

void (sprite_pic2src)(const sprite_t *p, int16_t u, int16_t v, int16_t *x, int16_t *y){
    int16_t du = u - basic_sprite_get_u0(p->bsp);
    int16_t dv = v - basic_sprite_get_v0(p->bsp);
    double dx =  du*p->c + dv*p->s;
    double dy = -du*p->s + dv*p->c;
    *x = dx*p->scale + 0.5 + p->x;
    *y = dy*p->scale + 0.5 + p->y;
}

void (sprite_draw)(const sprite_t *p){
    const uint16_t w = basic_sprite_get_w(p->bsp);
    const uint16_t h = basic_sprite_get_h(p->bsp);
    int16_t xmin, xmax, ymin, ymax; {
        int16_t x, y;
        sprite_pic2src(p, 0, 0, &x, &y);
        xmin = x; xmax = x; ymin = y; ymax = y;
        sprite_pic2src(p, w, 0, &x, &y);
        xmin = min(x, xmin); xmax = max(x, xmax); ymin = min(y, ymin); ymax = max(y, ymax);
        sprite_pic2src(p, 0, h, &x, &y);
        xmin = min(x, xmin); xmax = max(x, xmax); ymin = min(y, ymin); ymax = max(y, ymax);
        sprite_pic2src(p, w, h, &x, &y);
        xmin = min(x, xmin); xmax = max(x, xmax); ymin = min(y, ymin); ymax = max(y, ymax);
        xmin = max(xmin-2, 0); xmax = min(xmax+2, graph_get_XRes());
        ymin = max(ymin-2, 0); ymax = min(ymax+2, graph_get_YRes());
    }
    const uint8_t *map = basic_sprite_get_map(p->bsp);
    int16_t u, v;
    for(int16_t y = ymin; y < ymax; ++y){
        for(int16_t x = xmin; x < xmax; ++x){
            sprite_src2pic(p, x, y, &u, &v);
            if(0 <= u && u < w && 0 <= v && v < h){
                uint32_t c = *(uint32_t*)(map + (v*w + u)*4);
                if(GET_ALP(c) < 0x7F)
                    graph_set_pixel(x, y, GET_COLOR(c));
            }
        }
    }
}
