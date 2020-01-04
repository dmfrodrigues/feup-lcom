#include <lcom/lcf.h>

#include "rectangle.h"

#include "graph.h"
#include "utils.h"

struct rectangle{
    int16_t  x, y;
    uint16_t w, h;
    uint32_t outline_color;
    uint32_t fill_color;
    int16_t  outline_width;
    uint8_t  fill_alpha;
};

rectangle_t* (rectangle_ctor)(int16_t x, int16_t y, uint16_t w, uint16_t h){
    rectangle_t *ret = (rectangle_t*)malloc(sizeof(rectangle_t));
    if(ret == NULL) return NULL;
    ret->x = x;
    ret->y = y;
    ret->w = w;
    ret->h = h;
    ret->fill_color    = 0xFFFFFF;
    ret->fill_alpha    = 0xFF;
    ret->outline_color = 0x000000;
    ret->outline_width = 0;
    return ret;
}
void (rectangle_dtor)(rectangle_t *p){
    if(p == NULL) return;
    free(p);
}

void (rectangle_set_pos)          (rectangle_t *p,  int16_t x,  int16_t y){ p->x = x; p->y = y; }
void (rectangle_set_size)         (rectangle_t *p, uint16_t w, uint16_t h){ p->w = w; p->h = h; }
void (rectangle_set_fill_color)   (rectangle_t *p, uint32_t color        ){ p->fill_color = color; }
void (rectangle_set_fill_trans)   (rectangle_t *p, uint8_t  alpha        ){ p->fill_alpha = alpha; }
void (rectangle_set_outline_color)(rectangle_t *p, uint32_t color        ){ p->outline_color = color; }
void (rectangle_set_outline_width)(rectangle_t *p,  int16_t width        ){ p->outline_width = width; }

int16_t  (rectangle_get_x)(const rectangle_t *p){ return p->x; }
int16_t  (rectangle_get_y)(const rectangle_t *p){ return p->y; }
uint16_t (rectangle_get_w)(const rectangle_t *p){ return p->w; }
uint16_t (rectangle_get_h)(const rectangle_t *p){ return p->h; }

int (rectangle_collide_point)(const rectangle_t *p, int x, int y) {
    int16_t x0 = p->x, y0 = p->y;
    return (x >= x0 && x <= x0 + p->w) && (y >= y0 && y <= y0 + p->h);
}

static void (rectangle_draw_hline)(int16_t x, int16_t y, int16_t l, uint32_t color){
    if(l < 0){ rectangle_draw_hline(x+l, y, -l, color); return; }
    for(int16_t x_ = max16(0,x); x_ < min16(x+l,(int16_t)graph_get_XRes()); ++x_){
        graph_set_pixel((uint16_t)x_, (uint16_t)y, color);
    }
}
static void (rectangle_draw_vline)(int16_t x, int16_t y, int16_t l, uint32_t color){
    if(l < 0){ rectangle_draw_vline(x, y+l, -l, color); return; }
    for(int16_t y_ = max16(0,y); y_ < min16(y+l,(int16_t)graph_get_YRes()); ++y_){
        graph_set_pixel((uint16_t)x, (uint16_t)y_, color);
    }
}

void (rectangle_draw)(const rectangle_t *p){
    /// fill
    if(p->fill_alpha > ALPHA_THRESHOLD)
        for(int16_t y = max16(p->y,0); y < min16(p->y+p->h, (int16_t)graph_get_YRes()); ++y)
            rectangle_draw_hline(p->x, y, (int16_t)p->w, p->fill_color);
    /// border
    int16_t step = (p->outline_width > 0 ? 1 : -1);
    int16_t l = p->x, r = p->x+p->w, t = p->y, b = p->y+p->h;
    if(step > 0){
        --l; ++r; --t; ++b;
    }
    for(int16_t i = 0; i != p->outline_width; i += step, l -= step, r += step, t -= step, b += step){
        rectangle_draw_hline(l  , t  , r-l, p->outline_color);
        rectangle_draw_hline(l  , b-1, r-l, p->outline_color);
        rectangle_draw_vline(l  , t  , b-t, p->outline_color);
        rectangle_draw_vline(r-1, t  , b-t, p->outline_color);
    }
}
